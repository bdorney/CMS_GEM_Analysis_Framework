#!/bin/zsh

#Delete run config files
rm $FRAMEWORK_BASE/config/configRun_JobNo*.cfg

#Delete submission scripts
rm $FRAMEWORK_BASE/scripts/submitFrameworkJob_JobNo*.sh

#Delete std err, log, and out files
rm $FRAMEWORK_BASE/stderr/*.txt
rm $FRAMEWORK_BASE/stdlog/*.txt
rm $FRAMEWORK_BASE/stdout/*.txt
