#include <stdio.h>
#include <sysrepo.h>
#include <unistd.h>

int main(void) {
	sr_log_stderr(SR_LL_WRN);
	sr_conn_ctx_t *conn = 0;
	sr_session_ctx_t *sess = 0;
	int res;
	res = sr_connect(0, &conn);
	printf("connect result %d\n", res);
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
	if (conn) {
		sr_disconnect(conn);
	}
	return 0;
}
