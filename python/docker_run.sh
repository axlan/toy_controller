#!/usr/bin/env bash

docker build -t toy-controller-sonos-wled .
docker rm toy-controller
docker run -itd --restart=always --name toy-controller toy-controller-sonos-wled
