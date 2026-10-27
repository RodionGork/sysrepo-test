#include <stdio.h>
#include <sysrepo.h>
#include <unistd.h>

int onChange(sr_session_ctx_t *session, uint32_t sub_id, const char *module, const char *xpath,
        sr_event_t event, uint32_t request_id, void *private_data) {
	return SR_ERR_OK;
}

int main(void) {
	sr_log_stderr(SR_LL_WRN);
	sr_conn_ctx_t *conn = 0;
	sr_session_ctx_t *sessr = 0;
	sr_session_ctx_t *sess = 0;
	sr_subscription_ctx_t *subscr = 0;
    int res;
	res = sr_connect(0, &conn);
	printf("connect result %d\n", res);
	res = sr_session_start(conn, SR_DS_RUNNING, &sessr);
	printf("session-r start %d\n", res);
	res = sr_module_change_subscribe(sessr, "test-module", 0, onChange, 0, 0, SR_SUBSCR_ENABLED|SR_SUBSCR_DONE_ONLY, &subscr);
	printf("subscr res %d\n", res);
	res = sr_session_start(conn, SR_DS_OPERATIONAL, &sess);
	printf("session start %d\n", res);
	if (res == SR_ERR_OK) {
		res = sr_set_item_str(sess, "/test-module:elements/elem[id=13]/online", "true", 0, 0);
		printf("set data res %d\n", res);
		res = sr_apply_changes(sess, 0);
		printf("apply res %d\n", res);
	}
	printf("waiting for 2 seconds before stop...\n");
	sleep(2);
	if (sess)
		sr_session_stop(sess);
	if (subscr)
		sr_unsubscribe(subscr);
	if (sessr)
		sr_session_stop(sessr);
	if (conn)
		sr_disconnect(conn);
	return 0;
}
