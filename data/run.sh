#!/bin/sh -e

set -x

trang -I xml -O xsd *.ExamCard ../xsd/examcard.xsd
