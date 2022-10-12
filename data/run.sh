#!/bin/sh -e

set -x

trang -I xml -O xsd *.ExamCard ../xsd/examcard.xsd

# dogfooding
trang -I xml -O rng *.ExamCard examcard.rng

for xml in *.ExamCard; do
	jing examcard.rng $xml
done
