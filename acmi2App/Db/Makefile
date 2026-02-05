TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
DB += pulse_stats.db
DB += control.db
DB += lstats.db
DB += eeprom.db
DB += adc_data.db
# Non-PSC PV database file:
DB += TP.db
DB += ADC.db
DB += Beam.db
DB += Tail.db

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

