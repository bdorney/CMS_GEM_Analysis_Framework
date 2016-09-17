#!/bin/zsh

#Delete run config files
rm $GEM_BASE/config/configRun_RunNo*.cfg

#Delete submission scripts
rm $GEM_BASE/scripts/submitFrameworkJob_RunNo*.sh

#Delete std err, log, and out files
rm $GEM_BASE/stderr/*.txt
rm $GEM_BASE/stdlog/*.txt
rm $GEM_BASE/stdout/*.txt
