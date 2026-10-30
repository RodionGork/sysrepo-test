#include <stdio.h>
#include <sysrepo.h>
#include <unistd.h>

int onChange(sr_session_ctx_t *session, uint32_t sub_id, const char *module, const char *xpath,
        sr_event_t event, uint32_t request_id, void *private_data) {
	return SR_ERR_OK;
}

void fillSampleData(sr_session_ctx_t *s, int count, int offs, char *field) {
	char xpath[256];
	char name[32];
	for (int i = 0; i < count; i++) {
		sprintf(xpath, "/test-module:elements/elem[id=%d]/%s", i*10, field);
		sprintf(name, "%d", i*10000+offs);
		sr_set_item_str(s, xpath, name, 0, 0);
	}
	sr_apply_changes(s, 0);
}

void fillSampleStates(sr_session_ctx_t *s, int count, char *field) {
	char xpath[256];
	char name[32];
	for (int i = 0; i < count; i++) {
		sprintf(xpath, "/test-module:elements/elem[id=%d]/%s", i*10, field);
		sr_set_item_str(s, xpath, i % 2 ? "yes": "no", 0, 0);
	}
	sr_apply_changes(s, 0);
}

int main(int argc, char**argv) {
	if (argc < 5) {
		printf("Specify counts:\n%s <names> <comments> <onlines> <actives>\n",
				argv[0]);
		return 0;
	}
	sr_log_stderr(SR_LL_WRN);
	sr_conn_ctx_t *conn = 0;
	sr_session_ctx_t *sessr = 0, *sesso = 0;
	sr_subscription_ctx_t *subscr = 0;
    int res;
	res = sr_connect(0, &conn);
	printf("connect result %d\n", res);
	res = sr_session_start(conn, SR_DS_RUNNING, &sessr);
	printf("session-r start %d\n", res);
	res = sr_module_change_subscribe(sessr, "test-module", 0, onChange,
			0, 0, SR_SUBSCR_ENABLED|SR_SUBSCR_DONE_ONLY, &subscr);
	printf("subscr res %d\n", res);
	fillSampleData(sessr, atoi(argv[1]), 0, "name");
	fillSampleData(sessr, atoi(argv[2]), 7, "comment");
	res = sr_session_start(conn, SR_DS_OPERATIONAL, &sesso);
    fillSampleStates(sesso, atoi(argv[3]), "online");
    fillSampleStates(sesso, atoi(argv[4]), "active");
	fclose(fopen("proceed.flag", "wb"));
	printf("waiting for 2 seconds before stop...\n");
	sleep(2);
	printf("cleaning up sessions, subscriptions and connections\n");
	if (sesso)
		sr_session_stop(sesso);
	if (subscr)
		sr_unsubscribe(subscr);
	if (sessr)
		sr_session_stop(sessr);
	if (conn)
		sr_disconnect(conn);
	printf("all done\n");
	fclose(fopen("complete.flag", "wb"));
	return 0;
}
