#!/bin/bash
# TODO: rewrite using rsync

scp -r *.sh mgalazyn@bulk:msc/
scp -r macros/*.C mgalazyn@bulk:msc/macros/
scp -r inc/* mgalazyn@bulk:msc/inc/
scp -r src/* mgalazyn@bulk:msc/src/
scp -r data/*.conf mgalazyn@bulk:msc/data/
scp -r data/*.root mgalazyn@bulk:msc/data/
