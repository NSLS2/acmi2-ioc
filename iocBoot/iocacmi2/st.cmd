#!../../bin/linux-x86_64/acmi2

#- You may have to change zpsc to something else
#- everywhere it appears in this file

< envPaths

epicsEnvSet("IOCNAME","lab")
epicsEnvSet("UNIT","A")
epicsEnvSet("IOCDIR","/home/psctest/acmi2/acmi2-ioc/")

# PSC IP address
#epicsEnvSet("ACMI2_IP", "10.0.142.198"); 
epicsEnvSet("ACMI2_IP", "10.0.142.128"); 

#epicsEnvSet("BLEN",100000);        # Snapshot DMA Length

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/acmi2.dbd"
dbLoadDatabase "dbd/acmi2Subs.dbd"

acmi2_registerRecordDeviceDriver pdbbase

## Non-PSC database files:
dbLoadRecords("db/ADC.db", "P=$(IOCNAME), NO=$(UNIT)")
dbLoadRecords("db/TP.db", "P=$(IOCNAME), NO=$(UNIT), T=1")
dbLoadRecords("db/TP.db", "P=$(IOCNAME), NO=$(UNIT), T=2")
dbLoadRecords("db/TP.db", "P=$(IOCNAME), NO=$(UNIT), T=3")
dbLoadRecords("db/Beam.db", "P=$(IOCNAME), NO=$(UNIT)")
dbLoadRecords("db/Tail2.db", "P=$(IOCNAME), NO=$(UNIT)")

## Load record instances for PSC1
dbLoadRecords("db/lstats.db", "P=$(IOCNAME), NO=$(UNIT)")
dbLoadRecords("db/control.db", "P=$(IOCNAME), NO=$(UNIT)")
dbLoadRecords("db/pulse_stats.db", "P=$(IOCNAME), NO=$(UNIT)")
dbLoadRecords("db/eeprom.db", "P=$(IOCNAME), NO=$(UNIT)")
dbLoadRecords("db/adc_data.db", "P=$(IOCNAME), NO=$(UNIT)")

var(PSCDebug, 1)	#5 full debug

#psc1 Create the PSC
createPSC("PSCA", $(ACMI2_IP), 3000, 0)
setPSCSendBlockSize("PSCA", 70000, 70000)

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq reset_digout P=$(IOCNAME) NO=1 CHAN=1 &

epicsThreadSleep(1.0)

dbpf $(IOCNAME){ACMI:$(UNIT)}EVR:TrigNo-SP 32
dbpf $(IOCNAME){ACMI:$(UNIT)}EVR:TrigDly-SP 1
dbpf $(IOCNAME){ACMI:$(UNIT)}Settings:File-SP $(IOCDIR)settings/settings.txt
dbpf $(IOCNAME){ACMI:$(UNIT)}COW:DirPath-SP $(IOCDIR)COW/
dbpf $(IOCNAME){ACMI:$(UNIT)}ADC:Corr:DirPath-SP $(IOCDIR)Correction/
dbpf $(IOCNAME){ACMI:$(UNIT)}ADC:UpdatePV-SP $(IOCNAME){ACMI:$(UNIT)}ADC:UpdateTable-SP
