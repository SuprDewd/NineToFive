#include <check.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/timespec.h"

#define DATE_TIME_FMT "%Y-%m-%dT%H:%M:%SZ"

char timebuf[64];
char *str_resolve_timespec(char *relative_to, char *tspec) {
    struct tm relative_to_tm;
    if (strptime(relative_to, DATE_TIME_FMT, &relative_to_tm) == NULL) {
        ck_assert(0);
    }

    time_t relative_to_time = timegm(&relative_to_tm);
    time_t res = resolve_timespec(relative_to_time, tspec);
    if (res == (time_t)-1) {
        return NULL;
    }

    struct tm *res_tm = gmtime(&res);
    strftime(timebuf, 64, DATE_TIME_FMT, res_tm);
    return timebuf;
}

#define assert_str_eq(a, b) \
    ck_assert_int_eq((a) == NULL, (b) == NULL); \
    if ((a) != NULL && (b) != NULL) \
        ck_assert_str_eq((a), (b));

START_TEST(test_resolve_timespec) {

    /* Identity timespec */
    ck_assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", ""));

    /* Adding an interval */
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+0s"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T00:00:00Z", "+0:0s"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+0m"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+0:0m"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T00:00:00Z", "+0:0:0m"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+0h"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+0:0h"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+0:0:0h"));
    assert_str_eq("2014-01-01T00:00:01Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+1s"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T00:00:00Z", "+1:1s"));
    assert_str_eq("2014-01-01T00:01:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+1m"));
    assert_str_eq("2014-01-01T00:01:01Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+1:1m"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T00:00:00Z", "+1:1:1m"));
    assert_str_eq("2014-01-01T01:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+1h"));
    assert_str_eq("2014-01-01T01:01:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+1:1h"));
    assert_str_eq("2014-01-01T01:01:01Z", str_resolve_timespec("2014-01-01T00:00:00Z", "+1:1:1h"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T00:00:00Z", "+1:1:1:1h"));

    /* Subtracting an interval */
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "-0s"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T00:00:00Z", "-0:0s"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "-0m"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "-0:0m"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T00:00:00Z", "-0:0:0m"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "-0h"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "-0:0h"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T00:00:00Z", "-0:0:0h"));
    assert_str_eq("2014-01-01T01:01:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-1s"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "-1:1s"));
    assert_str_eq("2014-01-01T01:00:01Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-1m"));
    assert_str_eq("2014-01-01T01:00:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-1:1m"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "-1:1:1m"));
    assert_str_eq("2014-01-01T00:01:01Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-1h"));
    assert_str_eq("2014-01-01T00:00:01Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-1:1h"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-1:1:1h"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "-1:1:1:1h"));

    /* Specifying absolute time today */
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T01:02:03Z", "0"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T01:02:03Z", "0:0"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T01:02:03Z", "0:0:0"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:02:03Z", "0:0:0:0"));
    assert_str_eq("2014-01-01T08:00:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "8"));
    assert_str_eq("2014-01-01T08:13:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "8:13"));
    assert_str_eq("2014-01-01T08:13:20Z", str_resolve_timespec("2014-01-01T01:01:01Z", "8:13:20"));
    assert_str_eq("2014-01-01T16:00:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "16"));
    assert_str_eq("2014-01-01T16:13:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "16:13"));
    assert_str_eq("2014-01-01T16:13:20Z", str_resolve_timespec("2014-01-01T01:01:01Z", "16:13:20"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "24"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "23:60"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "23:59:60"));
    /* TODO: support AM/PM suffix */

    /* Next time that matches */
    assert_str_eq("2014-01-02T00:00:00Z", str_resolve_timespec("2014-01-01T01:02:03Z", "+0"));
    assert_str_eq("2014-01-02T00:00:00Z", str_resolve_timespec("2014-01-01T01:02:03Z", "+0:0"));
    assert_str_eq("2014-01-02T00:00:00Z", str_resolve_timespec("2014-01-01T01:02:03Z", "+0:0:0"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:02:03Z", "+0:0:0:0"));
    assert_str_eq("2014-01-02T01:00:00Z", str_resolve_timespec("2014-01-01T02:02:03Z", "+1"));
    assert_str_eq("2014-01-02T01:02:00Z", str_resolve_timespec("2014-01-01T02:02:03Z", "+1:2"));
    assert_str_eq("2014-01-02T01:02:03Z", str_resolve_timespec("2014-01-01T02:02:03Z", "+1:2:3"));
    assert_str_eq("2014-01-01T08:00:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "+8"));
    assert_str_eq("2014-01-01T08:13:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "+8:13"));
    assert_str_eq("2014-01-01T08:13:20Z", str_resolve_timespec("2014-01-01T01:01:01Z", "+8:13:20"));
    assert_str_eq("2014-01-01T16:00:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "+16"));
    assert_str_eq("2014-01-01T16:13:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "+16:13"));
    assert_str_eq("2014-01-01T16:13:20Z", str_resolve_timespec("2014-01-01T01:01:01Z", "+16:13:20"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "+24"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "+23:60"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "+23:59:60"));
    /* TODO: support AM/PM suffix */

    /* Previous time that matches */
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T01:02:03Z", "-0"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T01:02:03Z", "-0:0"));
    assert_str_eq("2014-01-01T00:00:00Z", str_resolve_timespec("2014-01-01T01:02:03Z", "-0:0:0"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:02:03Z", "-0:0:0:0"));
    assert_str_eq("2014-01-01T01:00:00Z", str_resolve_timespec("2014-01-01T02:02:03Z", "-1"));
    assert_str_eq("2014-01-01T01:02:00Z", str_resolve_timespec("2014-01-01T02:02:03Z", "-1:2"));
    assert_str_eq("2014-01-01T01:02:03Z", str_resolve_timespec("2014-01-01T02:02:03Z", "-1:2:3"));
    assert_str_eq("2013-12-31T08:00:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-8"));
    assert_str_eq("2013-12-31T08:13:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-8:13"));
    assert_str_eq("2013-12-31T08:13:20Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-8:13:20"));
    assert_str_eq("2013-12-31T16:00:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-16"));
    assert_str_eq("2013-12-31T16:13:00Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-16:13"));
    assert_str_eq("2013-12-31T16:13:20Z", str_resolve_timespec("2014-01-01T01:01:01Z", "-16:13:20"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "-24"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "-23:60"));
    assert_str_eq(NULL, str_resolve_timespec("2014-01-01T01:01:01Z", "-23:59:60"));
    /* TODO: support AM/PM suffix */

    /* Specifying absolute date and time */
    /* assert_str_eq("2013-16-23T03:28:17Z", str_resolve_timespec("2014-01-01T01:02:03Z", "2013-16-23T03:28:17Z")); */
}
END_TEST


Suite *timespec_suite(void) {
  Suite *s = suite_create("timespec");

  TCase *tc_parsing = tcase_create("Parsing");
  tcase_add_test(tc_parsing, test_resolve_timespec);
  suite_add_tcase(s, tc_parsing);

  return s;
}


int main (void) {
  int number_failed;
  Suite *s = timespec_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
