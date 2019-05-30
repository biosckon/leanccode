#include "../all.h"

#define enable 1


int main() {
    CURLM *mhand = curl_multi_init();
    CURL *hand = curl_easy_init();

    curl_multi_add_handle(mhand, hand);

    CURLcode res = curl_easy_setopt(hand, CURLOPT_URL,
            "https://google.com");
    res = curl_easy_setopt(hand, CURLOPT_FOLLOWLOCATION, enable);

    int transfer_run;
    do {
        curl_multi_wait(mhand, NULL, 0, 1000, NULL);
        curl_multi_perform(mhand, &transfer_run);
    } while (transfer_run);

    printf("ret %d\n", res);

    // done
    curl_multi_remove_handle(mhand, hand);
    curl_easy_cleanup(hand);
    curl_multi_cleanup(mhand);
}
