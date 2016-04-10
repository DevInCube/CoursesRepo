#ifndef WEBPAGE_H_INCLUDED
#define WEBPAGE_H_INCLUDED

typedef struct webpage_s webpage_t;

typedef
    void
    (*webpage_notification_fn)
    (void * receiver, webpage_t * sender, const char * message, const char * hyperlink);

typedef
    void
    (*webpage_linkclick_fn)
    (void * receiver, webpage_t * sender, const char * hyperlink);

webpage_t *
webpage_new(const char * aName);

void
webpage_free(webpage_t * self);

const char *
webpage_getName(webpage_t * self);

void
webpage_subscribeNotification(webpage_t * self, void * receiver, webpage_notification_fn callback);

void
webpage_subscribeLinkClick(webpage_t * self, void * receiver, webpage_linkclick_fn callback);

void
webpage_sendMessage(webpage_t * self, const char * message, const char * hyperlink);

void
webpage_clickLink(webpage_t * self, const char * hyperlink);

#endif // WEBPAGE_H_INCLUDED
