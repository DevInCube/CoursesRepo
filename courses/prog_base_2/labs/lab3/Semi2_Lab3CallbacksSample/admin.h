#ifndef ADMIN_H_INCLUDED
#define ADMIN_H_INCLUDED

#include "webpage.h"

typedef struct admin_s admin_t;

admin_t *
admin_new(const char * name);

void
admin_free(admin_t * self);

void
admin_setWebpage(admin_t * self, webpage_t * page);

webpage_linkclick_fn
admin_getLinkClickCallback(admin_t * self);

void
admin_sendWebpageNotification(admin_t * sender, const char * message, const char * hyperlink);

#endif // ADMIN_H_INCLUDED
