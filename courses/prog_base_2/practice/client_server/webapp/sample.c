/* 
	- static files support ? url_for('static', filename='style.css') 

	Extra:
		- unit-tests for all modules with CMocka
		- sqlite\postgres module
		- sessions
*/

#include <stdlib.h>
#include <stdio.h>

/* COLLECTIONS.H */
typedef struct map_s map_t;		// void * values
typedef struct list_s list_t;	// void * values

/* HTTP.H */

// Flags
typedef enum http_method {
	HTTP_GET 		= 1,
	HTTP_HEAD 		= 1 << 1,
	HTTP_POST 		= 1 << 2,
	HTTP_PUT 		= 1 << 3,
	HTTP_DELETE 	= 1 << 4,
	HTTP_OPTIONS 	= 1 << 5
} http_method_t;

/* APP.H */
//
// web server app type
typedef struct app_s app_t;
//
// http request type
typedef struct request_s request_t;
//
// http response type
typedef struct response_s response_t;
// 
// http request user handler function pointer
typedef response_t * (*request_handler_fn)(app_t * app, request_t * req);

app_t * app_new(void);
void app_free(app_t * self);
//
// returns old user context
void * app_setContext(app_t * self, void * ctx) __attribute__((nonnull (1)));
void * app_getContext(app_t * self) __attribute__((nonnull (1)));
//
// set user handler function for uri request with defined methods
void app_route(app_t * self, const char * uri, request_handler_fn handler, http_method_t methods);
//
// default port (5000) on localhost
void app_run(app_t * self); 
void app_run_port(app_t * self, int port);
void app_run_ip_port(app_t * self, const char * ipAddr, int port);

response_t * response_new(void);
void response_free(response_t * self);
void response_setStatus(response_t * self, int statusCode); 
void response_setHeader(response_t * self, const char * key, const char * value); 
void response_setCookie(response_t * self, const char * key, const char * value); 
void response_redirect(response_t * self, const char * redirectUrl); 

/* TEMPLATE.H */

// 
// native support for logicless HTML template engine like Mustache https://github.com/no1msd/mstch [C++]
// partials support https://github.com/no1msd/mstch#partials
// lambda-like support https://github.com/no1msd/mstch#lambdas
//
// free this string if not set to response
const char * render_template(const char * tmplFileName, const map_t * tmplData);

/* MAIN.C */

// #include "app.h"

// user-defined context struct
typedef struct context_s {
	/* any fields */
	int test;
} context_t;

// request handlers
static response_t * handle_homepage(app_t * app, request_t * req);
static response_t * handle_students(app_t * app, request_t * req);
static response_t * handle_studentById(app_t * app, request_t * req);

int main(void) {
	context_t ctx = {
		/* init data */ .test = 0
	};
	app_t * app = app_new();
	app_setContext(app, &ctx);
	app_route(app, "/", handle_homepage, HTTP_GET);
	app_route(app, "/students", handle_students, HTTP_GET | HTTP_POST);
	// 
	// <student_id> is string variable in request URL
	app_route(app, "/students/<student_id>", handle_studentById, HTTP_GET | HTTP_POST | HTTP_PUT);  
	app_run(app);
	app_free(app);
	return 0;
}

static response_t * handle_homepage(app_t * app, request_t * req) {
	// "template.html" should be in /templates folder
	// map_t * someData = { ... };
	// render_template("template.html", someData);
	return response_new();
}

static response_t * handle_students(app_t * app, request_t * req) {
	/**/
	return response_new();
}

static response_t * handle_studentById(app_t * app, request_t * req) {
	/* get student_id string by name from req object */
	return response_new();
}

/* APP.C */
struct app_s {
	/* smth */
	void * userContext;
};

struct request_s {
	map_t * const args;		// URI args
	map_t * const form;		// POST args
	map_t * const headers;
	map_t * const files;
	map_t * const cookies;
};

struct response_s {
	int statusCode;			// default 200 OK
	char * data;
	int dataLength;
	map_t * headers;
	char * redirectURL;  	// default NULL
};

app_t * app_new(void) {
	/* */
}
void app_free(app_t * self) {
	/* */
}
void * app_setContext(app_t * self, void * ctx) {
	/* */
}
void * app_getContext(app_t * self) {
	/* */
}
void app_route(app_t * self, const char * uri, request_handler_fn handler, http_method_t methods) {
	/* */
}
void app_run(app_t * self) {
	/* */
}

/* REQUEST.C */

// hidden constructor and destructor
static request_t * request_new(void);
static void request_free(request_t *);

/* RESPONSE.C */
response_t * response_new(void) {
	/* */
}
	
void response_free(response_t * self) {
	/* */
}
