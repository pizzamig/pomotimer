#!/usr/bin/env sh

if [ ! -r log ]; then
	exit 1
fi

if [ $(grep -c FAILURE log) -ne 0 ]; then
	exit 1
fi
