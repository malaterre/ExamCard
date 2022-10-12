#!/bin/sh -e

set -x

trang -I xml -O xsd *.ExamCard examcard.xsd
