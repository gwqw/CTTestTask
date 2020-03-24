# READ ME [![Build Status](https://travis-ci.org/gwqw/CTTestTask.svg?branch=master)](https://travis-ci.org/github/gwqw/CTTestTask)


`fps_framesdata` -- console application for parsing episode logs (yaml-files compressed to .gz) and average fps calculation for every caption.

## Command line

```sh
-d <Path>, --dir <Path>	Input directory with *.info.yaml.gz files
-s <start_episode>, --start <start_episode> episode number to start counting fps. Empty means from the first in directory
-e <end_episode>, --end <end_episode> episode number to stop counting fps. Empty means to the last in directory (inclusive)
-o <output_filename> output filename (csv-table). Empty means output to console
-h, --help	this screen
```



## Dependency

- `openCV` is used command options parsing, for yaml-files (in .gz) parsing and for searching files.



## YAML-file description

`name.xxx.nnn.info.yml`

`name` -- passageway name

`xxx` -- passageway number

`nnn` -- episode number

*Example*: `bui.004.001.info.yml.gz`

Tag **header**

capture list: `capture1`, `capture2`, ..., `captureN`

`capture` has `name`

Tag **shots**

`shot`s list: 

`shot` has fields

 - grabNumber -- shot number
 - grabMsec -- shot timestamp in ms
 - list of `captures`:
   	- `name` -- capture name (same as in header)
    - `grabMsec` -- timestamps from application start in ms



