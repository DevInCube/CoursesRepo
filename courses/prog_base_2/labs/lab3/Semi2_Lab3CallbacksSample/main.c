#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "webpage.h"
#include "admin.h"

typedef struct user_s {
    char name[100];
} user_t;

void user_onNotification(void * receiver, webpage_t * sender, const char * message, const char * hyperlink) {
    user_t * user = (user_t *)receiver;
    const char * pageName = webpage_getName(sender);
    printf("User '%s' received:\n\tMessage:\t'%s'\n\t   Link:\t'%s'\n\t Sender:\t'%s'\n\n",
           user->name,
           message,
           hyperlink,
           pageName);
    int randomNum = rand() % 3;
    if (0 != randomNum) {
        printf("User '%s' clicked link: '%s'\n\n", user->name, hyperlink);
        webpage_clickLink(sender, hyperlink);
    }
}

int main(void) {
    srand(time(NULL));
    // create webpage
    webpage_t * progbase = webpage_new("prog-base.appspot.com");
    // subscribe admin
    admin_t * mrBig = admin_new("Mr. Big");
    admin_setWebpage(mrBig, progbase);
    webpage_subscribeLinkClick(progbase, mrBig, admin_getLinkClickCallback(mrBig));
    //subscribe users
    user_t users[4] = {{"Taras"}, {"Olya"}, {"Ira"}, {"Andriy"}};
    for (int i = 0; i < 4; i++) {
        webpage_subscribeNotification(progbase, &users[i], user_onNotification);
    }
    // send message
    admin_sendWebpageNotification(mrBig, "Hello, humanz!", "some.page.com");
    webpage_clickLink(progbase, "Hyper.com");
    //
    admin_free(mrBig);
    webpage_free(progbase);
    return 0;
}

