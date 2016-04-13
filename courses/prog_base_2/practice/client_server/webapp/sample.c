/*
	unit-tests for all modules with CMocka
*/

#include <stdlib.h>
#include <stdio.h>

// #include "app.h"

/* APP.H */
typedef struct app_s app_t;
typedef struct request_s request_t;
typedef struct response_s response_t;
typedef response_t * (*request_handler_fn)(app_t * app, request_t * req, void * ctx);

/* static files support ? url_for('static', filename='style.css') */
app_t * app_new(void);
void app_bind(app_t * self, int port);
void app_setContext(app_t * self, void * ctx);
void app_on(app_t * self, const char * uri, request_handler_fn handler, const char * methods[]);
void app_run(app_t * self); 
/* void app_run_ip(app_t * self, const char * ipAddr); */
void app_free(app_t * self); 

response_t * response_new(void);
void response_free(response_t * self);
/* void response_setStatus(response_t * self, int statusCode); */
/* void response_setHeader(response_t * self, const char * key, const char * value); */
/* void response_set_cookie(response_t * self, const char * key, const char * value); */
/* void response_redirect(response_t * self, const char * redirectUrl); */

/* TEMPLATE.H */

// 
// native support for logicless text template engine like https://github.com/no1msd/mstch [C++]
// partials support https://github.com/no1msd/mstch#partials
// lambda-like support https://github.com/no1msd/mstch#lambdas
/* const char * render_template(const char * templFileName, ?some_dict_type? template_data); */

/* MAIN.C */
typedef struct context_s {
	/* any fields */
} context_t;

static response_t * handle_homepage(app_t * app, request_t * req, void * ctx);
static response_t * handle_students(app_t * app, request_t * req, void * ctx);
static response_t * handle_studentById(app_t * app, request_t * req, void * ctx);

int main(void) {
	context_t ctx = {
		/* init data*/
	};
	app_t * app = app_new();
	app_bind(app, 5000);
	app_setContext(app, &ctx);
	app_on(app, "/", handle_homepage, (const char * []) {"GET", NULL});
	app_on(app, "/students", handle_students, (const char * []) {"GET", "POST", NULL});
	app_on(app, "/students/<int:student_id>", handle_studentById, (const char * []) {"GET", "POST", NULL});
	app_run(app);
	app_free(app);
	return 0;
}

static response_t * handle_homepage(app_t * app, request_t * req, void * ctx) {
	/**/
	// template.html should be in /templates folder
	// return response_new_render_template("template.html", template_data);  // template_data - ?
	return response_new();
}

static response_t * handle_students(app_t * app, request_t * req, void * ctx) {
	/**/
	return response_new();
}

static response_t * handle_studentById(app_t * app, request_t * req, void * ctx) {
	/* get student_id integer by name from req object */
	return response_new();
}

/* web app module */

/* APP.C */
struct app_s {
	/* smth */
	void * userContext;
};

struct request_s {
	/* smth */
	/* GET URI args dict */
	/* POST form dict */
	/* headers dict */
	/* files dict */
	/* cookies dict */
};

struct response_s {
	/* smth */
	/* status code */
	/* body data */
	/* headers dict */
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

/* response.C */
response_t * response_new(void) {
	/* */
}
	
void response_free(response_t * self) {
	/* */
}
