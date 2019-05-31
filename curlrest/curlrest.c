#include "../all.h"

// https://curl.haxx.se/libcurl/c/getinmemory.html
// https://gist.github.com/alan-mushi/19546a0e2c6bd4e059fd

#define enable 1

typedef struct {
    char *memory;
    size_t size;
} mem_struct;

void procJSON(mem_struct * buf) {
    json_t *root;
    json_error_t err;

    root = json_loads(buf->memory, 0, &err);
    if (!root) {
        fprintf(stderr, "error: on line %d: %s\n",
                err.line, err.text);
        return;
    }

    // can free buf->memory if want to

    for (int i = 0; i < json_array_size(root); i += 1) {
        json_t *data = json_array_get(root, i);
        json_t *ts = json_object_get(data, "timestamp");
        json_t *etim = json_object_get(data, "ETIM");
        printf("TS: %13.0f; ETIM: %10.5f\n", 
            json_real_value(ts),
            json_real_value(etim)
        );
    }
}

size_t writecb(char *content, size_t size, size_t nmemb, void *userp) {
    size_t actsize = size * nmemb;

    mem_struct *mem = (mem_struct *) userp;
    char *newmem = realloc(mem->memory, mem->size + actsize + 1);
    if (newmem == NULL) {
        printf("not enough mem\n");
        return 0;
    }

    mem->memory = newmem;
    memcpy(&(mem->memory[mem->size]), content, actsize);
    mem->size += actsize;
    mem->memory[mem->size] = 0; // add NULL terminator
    
    return actsize;
}

int main() {

    // memory buf
    mem_struct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    // init CURL
    curl_global_init(CURL_GLOBAL_ALL);
    CURLM *mhand = curl_multi_init();
    CURL *hand = curl_easy_init();

    curl_multi_add_handle(mhand, hand);

    CURLcode res;

    res = curl_easy_setopt(
            hand, CURLOPT_URL,
            "https://shellgamechanger.intelie.com/"
            "open/plugin-liverig/liverig-simplerest/pretest_monitor/"
            "query/well0?username=pretest&password=pretest");
    res = curl_easy_setopt(hand, CURLOPT_FOLLOWLOCATION, enable);
    res = curl_easy_setopt(hand, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    // register write callback and buffer
    res = curl_easy_setopt(hand, CURLOPT_WRITEFUNCTION, writecb);
    res = curl_easy_setopt(hand, CURLOPT_WRITEDATA, (void *)&chunk);
 
    int transfer_run;
    do {
        res = curl_multi_wait(mhand, NULL, 0, 1000, NULL);
        res = curl_multi_perform(mhand, &transfer_run);
    } while (transfer_run);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "failed: %s\n",
                curl_easy_strerror(res));
    } else {
        printf("%lu retrieved\n", (size_t)chunk.size);
    }

    procJSON(&chunk);

    // cleanup

    free(chunk.memory);
    chunk.size = 0;

    curl_multi_remove_handle(mhand, hand);
    curl_easy_cleanup(hand);
    curl_multi_cleanup(mhand);
    curl_global_cleanup();
}
