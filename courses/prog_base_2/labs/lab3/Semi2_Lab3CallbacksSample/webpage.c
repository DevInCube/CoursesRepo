#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "webpage.h"
#include "list.h"

struct webpage_s {
    char       name[256];
    list_t *   event_notification;
    event_t *  event_linkclick;
};

webpage_t *
webpage_new(const char * aName) {
    webpage_t * self = malloc(sizeof(struct webpage_s));
    strcpy(self->name, aName);
    self->event_notification = list_new();
    self->event_linkclick = NULL;
    return self;
}

void
webpage_free(webpage_t * self) {
    int count = list_getCount(self->event_notification);
    for (int i = 0; i < count; i++) {
        event_t * ev = list_pop_back(self->event_notification);
        free(ev);
    }
    list_free(self->event_notification);
    free(self->event_linkclick);
    free(self);
}

const char *
webpage_getName(webpage_t * self) {
    return self->name;
}

void
webpage_sendMessage(webpage_t * self, const char * message, const char * hyperlink) {
    int count = list_getCount(self->event_notification);
    for (int i = 0; i < count; i++) {
        event_t * ev = list_getEl(self->event_notification, i);
        if (NULL != ev->callback) {
            webpage_notification_fn fn = ev->callback;
            fn(ev->receiver, self, message, hyperlink);
        }
    }
}

void
webpage_subscribeNotification(webpage_t * self, void * receiver, webpage_notification_fn callback) {
    event_t * sb = malloc(sizeof(struct event_s));
    sb->receiver = receiver;
    sb->callback = callback;
    list_push_back(self->event_notification, sb);
}

void
webpage_subscribeLinkClick(webpage_t * self, void * receiver, webpage_linkclick_fn callback) {
    self->event_linkclick = malloc(sizeof(struct event_s));
    self->event_linkclick->receiver = receiver;
    self->event_linkclick->callback = callback;
}

void
webpage_clickLink(webpage_t * self, const char * hyperlink) {
    if ((NULL != self->event_linkclick)
        && (NULL != self->event_linkclick->callback)) {
        webpage_linkclick_fn fn = (webpage_linkclick_fn)self->event_linkclick->callback;
        fn(self->event_linkclick->receiver, self, hyperlink);
    }
}
