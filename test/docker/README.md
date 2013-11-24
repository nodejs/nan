## Running NAN tests in Docker containers

NAN needs to be tested across many versions of Node, so instead of messing with *nvm* and other version switching solutions, we can run the tests in separate Docker containers with their own versions of Node installed. The benefits of this approach are: it's easy to run multiple tests simultaneously and it's also easy to automate testing of Node *"master"*.

See the file ***node_versions.list*** for the list of versions that we're testing with.

See the file ***setup-docker.sh*** for setting up the Docker images. It should be as simple as [installing Docker](http://docs.docker.io/en/latest/installation/) and running the script (as root, i.e. `sudo ./setup-docker.sh`). You may need to `sudo docker pull ubuntu` first to make sure you have the base Ubuntu image to work with. Then each time you want to add a new version to test, edit the *node_versions.list* file and re-run *setup-docker.sh* and it'll add any missing images. If you want to update ***master*** then run `sudo docker rmi node_dev-master` and re-run *setup-docker.sh* and it'll pull the latest master and rebuild the image for you.

See the file ***run-docker.sh*** the the test invocation script. Use `sudo ./run-docker.sh` to execute the tests in the Docker containers. The script limits the number of simultaneous tests but you can edit that value. Output is saved to */tmp/* for each run and a basic **PASS** and **FAIL** is provided by parsing the end of the *tap* output.