# Sysrepo-Test

Build the docker image with `docker-build.sh`.

Start it with something like this:

    docker run -it --rm -v $(pwd):/app -w/app sysrepo-test

Download and build suitable libyang and sysrepo using the script inside, e.g.:

    ./fetch-and-build.sh 3.7.8 3.3.10

Run `test.sh` now, check results, then it is ok to exit.

### Observations

Sequential read and write is tolerably fast and proportional to input size,
e.g. 200k elements with 3 leaves each take `17` seconds to ingest (using
separate 600k `sr_set_item_str` calls), then export (via `sysrepocfg`) takes
`3.6` seconds.

Operational data access takes more time.

Speed was noticeably improved around the version 3.7 (and keeps without much
change to 5.2)

                                             3.3.10     5.2.27
    writing 100k config-true leaves           2800ms     2200ms
    writing same plus 100k oper leaves        6300ms     4100ms
    exporting 100k conf-true leaves            580ms      450ms
    exporting same plus 100k oper leaves      1200ms     1100ms

Random-read with separate `sr_get_item` is slow and degrades with
data size: reading `1000` fields from runtime, when the database contains only
`1000` elements takes `1.8` seconds, the same `1000` is however read in `3.8`
seconds when total `2000` elements are stored and even `7.6` seconds with `4000`.

Again for operational data this situation is significantly worse e.g. again
`1000` reads takes `7.9` seconds when only `1000` records are present but this
increases to `16.2` seconds for storage of `2000` records.
