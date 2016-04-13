#include <stdlib.h>
#include <stdio.h>

// #include "app.h"

/* APP.H */
typedef struct app_s app_t;
typedef struct request_s request_t;
typedef void (*request_handler_fn)(app_t * app, request_t * req, void * ctx);

app_t * app_new(void);
void app_bind(app_t * self, int port);
void app_setContext(app_t * self, void * ctx);
void app_on(app_t * self, const char * uri, request_handler_fn handler, const char * methods[]);
void app_run(app_t * self); 
void app_free(app_t * self); 

typedef struct context_s {
	/* any fields */
} context_t;

static void handle_homepage(app_t * app, request_t * req, void * ctx);
static void handle_students(app_t * app, request_t * req, void * ctx);

int main(void) {
	context_t ctx = {
		/* init data*/
	};
	app_t * app = app_new();
	app_bind(app, 5000);
	app_setContext(app, &ctx);
	app_on(app, "/", handle_homepage, (const char * []) {"GET", NULL});
	app_on(app, "/students", handle_students, (const char * []) {"GET", "POST", NULL});
	app_run(app);
	app_free(app);
	return 0;
}

static void handle_homepage(app_t * app, request_t * req, void * ctx) {
	/**/
}

static void handle_students(app_t * app, request_t * req, void * ctx) {
	/**/
}

/* web app module */

/* APP.C */
struct app_s {
	/* smth */
	void * userContext;
};

struct request_s {
	/* smth */
};

app_t * app_new(void) {
	/* */
}
void app_bind(app_t * self, int port) {
	/* */
}
void app_setContext(app_t * self, void * ctx) {
	/* */
}
void app_on(app_t * self, const char * uri, request_handler_fn handler, const char * methods[]) {
	/* */
}
void app_run(app_t * self) {
	/* */
}
void app_free(app_t * self) {
	/* */
}
