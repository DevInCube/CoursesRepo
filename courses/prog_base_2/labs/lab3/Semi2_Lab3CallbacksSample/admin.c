#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "admin.h"
#include "webpage.h"

struct admin_s {
    char name[100];
    webpage_t * webpage;
    int totalLinksClicked;
};

admin_t *
admin_new(const char * name) {
    admin_t * self = malloc(sizeof(struct admin_s));
    strcpy(self->name, name);
    self->totalLinksClicked = 0;
    return self;
}

void
admin_free(admin_t * self) {
    free(self);
}

void
admin_setWebpage(admin_t * self, webpage_t * page) {
    self->webpage = page;
}

static void
admin_onLinkClicked(void * receiver, webpage_t * sender, const char * hyperlink) {
    admin_t * self = (admin_t *)receiver;
    self->totalLinksClicked += 1;
    printf("Admin '%s' received event:\n", self->name);
    printf("\tSomeone clicked on '%s'\n", hyperlink);
    printf("\tTotal links clicked: %i\n\n", self->totalLinksClicked);
}

webpage_linkclick_fn
admin_getLinkClickCallback(admin_t * self) {
    return &admin_onLinkClicked;
}

void
admin_sendWebpageNotification(admin_t * self, const char * message, const char * hyperlink) {
    printf("Admin '%s' sent message to '%s':\n",
           self->name, webpage_getName(self->webpage));
    printf("\tMessage:\t'%s'\n\t   Link:\t'%s'\n\n", message, hyperlink);
    webpage_sendMessage(self->webpage, message, hyperlink);
}
