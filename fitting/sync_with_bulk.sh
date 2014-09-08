#!/bin/bash
# TODO: rewrite using rsync

scp -r *.sh mgalazyn@bulk:msc/fitting/
scp -r macros/*.C mgalazyn@bulk:msc/fitting/macros/
scp -r inc/* mgalazyn@bulk:msc/fitting/inc/
scp -r src/* mgalazyn@bulk:msc/fitting/src/
scp -r data/*.conf mgalazyn@bulk:msc/fitting/data/
scp -r data/*.root mgalazyn@bulk:msc/fitting/data/
