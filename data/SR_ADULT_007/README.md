% ./xpath1 ../../SR_ADULT_007.ExamCard "//SOAP-ENC:Array[@id='ref-297']" SOAP-ENC=http://schemas.xmlsoap.org/soap/encoding/

 % ./xpath1 ../../SR_ADULT_007.ExamCard "//foo:ScanProcedure" foo=http://schemas.microsoft.com/clr/nsassem/Philips.PmsMR.Acquisition.AcqGlo/philips.pmsmr.acquisition.acqglo_cs
Result (21 nodes):

./xpath1 ../../SR_ADULT_007.ExamCard "//foo:ScanProcedure/parameterData" foo=http://schemas.microsoft.com/clr/nsassem/Philips.PmsMR.Acquisition.AcqGlo/philips.pmsmr.acquisition.acqglo_cs
./xpath1 ../../SR_ADULT_007.ExamCard "//foo:ScanProcedure/name" foo=http://schemas.microsoft.com/clr/nsassem/Philips.PmsMR.Acquisition.AcqGlo/philips.pmsmr.acquisition.acqglo_cs

syntax:
https://www.freeformatter.com/xpath-tester.html

./xpath1 ../../SR_ADULT_007.ExamCard "//foo:ScanProcedure/*[name()='parameterData' or name()='name']" foo=http://schemas.microsoft.com/clr/nsassem/Philips.PmsMR.Acquisition.AcqGlo/philips.pmsmr.acquisition.
acqglo_cs

Ex:

./dump B1MAP_SAG_TR51_TR120_60DEG_SPOIL120.raw   

./dump Quiet_Survey.raw
./examcard2xml ../../SR_ADULT_007.ExamCard
