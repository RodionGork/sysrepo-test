# Sysrepo-Test

Build the docker image with `docker-build.sh`.

Start it with something like this:

    docker run -it --rm -v $(pwd):/app -w/app sysrepo-test

Download and build suitable libyang and sysrepo using the script inside, e.g.:

    ./fetch-and-build.sh 3.7.8 3.3.10

Run `test.sh` now, check results, then it is ok to exit.
