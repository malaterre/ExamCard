root=https://raw.githubusercontent.com/mrjeffstevenson3/mmimproc/master/data/testdata/examcard/

for i in PHANTOM_QT1_SLU_20151230.ExamCard SR_ADULT_007.ExamCard SR_ADULT_018.ExamCard TADPOLE_CHILD_V3.ExamCard sr_adult_007_examcard.xml sr_adult_007_examcard_scan_DTI6_b1_PA_TOPDN.xml; do
	wget -c $root/$i
done

wget -c https://raw.githubusercontent.com/spine-generic/protocols/main/Philips/spine_generic_philips_R53.ExamCard
wget -c https://raw.githubusercontent.com/spine-generic/protocols/main/Philips/spine_generic_philips_R53.pdf

wget -c https://raw.githubusercontent.com/VUIIS/examcardtotxt/main/examcard2txt/parameterPage.pm
