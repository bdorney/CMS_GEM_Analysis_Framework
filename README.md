# CMS_GEM_Analysis_Framework
Software tools for the analysis of experimental data collected by the CMS GEM community

Designed to work on `lxplus` running slc6 with `ROOT` version `6.00.02` or higher, `g++` version `4.8.4`, and `python` version `2.7.4`.

Instructions below assume the user is using a `sh/bash/zsh` shell.  Scripts for `csh/tsch` have not been implemented yet.

If you are having problems please consult the [closed issue page](https://github.com/bdorney/CMS_GEM_Analysis_Framework/issues?utf8=✓&q=is%3Aissue%20is%3Aclosed) to see if your issue has been solved before. If not please open a new issue [here](https://github.com/bdorney/CMS_GEM_Analysis_Framework/issues) and submit an issue that describes your problem following the examples in the issue template.  You may need to provide any input files or a link to the fork of the repository for us to troubleshoot.

# 1. Contributors & License
This repository is the work of:

- Main Developer: Brian Dorney
- Contributors: Marcello Maggi
- amoreSRS Team:  Kondo Gnanvo, Mike Staib, Stefano Colafranchescci, Dorothea Pfeiffer

This package has been designed by B. Dorney with input from J. Merlin & S. Colafranceschi. The event unpacking and reconstruction code was ported from `amoreSRS` by Marcello Maggi. The original selection & analysis algorithms are based off work done by J. Merlin.  This package makes use of several features from the `CMS_GEM_TB_Timing` repository (also by B. Dorney).

This package has adapted the `tdrstyle.C`, `CMS_lumi.h`, and `CMS_lumi.C` scripts (taken from [here](https://ghm.web.cern.ch/ghm/plots/)), for setting the style of output plots to match with the official guidelines of the CMS Experiment.

This package makes use of the `C++ Mathematical Expression Library` designed by Arash Partow, available [here](http://www.partow.net/programming/exprtk/index.html), and referred to as `ExprTk`. `ExprTk` is available under the "Common Public License" from the the url above, but has been included in this repository for convenience.

Additionally as a convenience for the user we have included the `get-pip.py` script taken from [here](https://packaging.python.org/installing/#install-pip-setuptools-and-wheel). This will handle the installation of `pip` in `python` version `2.7.4` automaticaly.

The `CMS_GEM_Analysis_Framework` is licensed under the "GNU General Public License."

# 2. Installation Instructions
This repository follows the guidelines of Vincent Driessen in [A successful Git branching model](http://nvie.com/posts/a-successful-git-branching-model/). The source code available in the `origin/master` branch always reflects a "production-ready" state.  The `origin/develop` branch reflects a state with the latest implemented changes for the next release.  This branch is not gauranteed to be stable.

To checkout the repository:

```
git clone https://github.com/bdorney/CMS_GEM_Analysis_Framework.git
cd CMS_GEM_Analysis_Framework
git pull origin master
```

You will now have the latest version of the `master` branch.  To compile the repository:

```
source scripts/setup_CMS_GEM.sh
make -f Makefile.gpp clean
make -f Makefile.gpp
make -f MakefilePlotter.gpp
```

The repository is now compiled.  Please note the first execution of `scripts/setup_CMS_GEM.sh` might make a local installation of `pip` and several other `python` packages that are required for the python analysis tools described in Section 3.c.  Additionally the base directory of the repository has been exported to the shell variable `$GEM_BASE`.

Please check [here](https://github.com/bdorney/CMS_GEM_Analysis_Framework) for the most-up-to-date release.  You migrate your `master` branch to the most-up-to-date branch via:

```
git checkout -b <local_branch_name>
git pull origin <remote_branch_name>
make -f Makefile.gpp clean
make -f Makefile.gpp
make -f MakefilePlotter.gpp
```

The `local_branch_name` is the name of the branch on your local machine. The `remote_branch_name` is the name of the branch you are checking out from the remote repository. It is a good practice to make `local_branch_name = remote_branch_name`.  You can see the list of available branches from command line via:

```
git fetch
git branch -a
```

The branch you are currently on will have the `*` character next to it.

NOTE: a make file for clang has been included "Makefile.clang" for MAC OS users.  However presently there is no support for any installation/runtime errors on a MAC OS environemnt. It is strongly urged that you use the Linux computing environment mentioned above (since it is so readily available to us).

# 3. Usage
The following gives the usage case for each of the executables produced during the installation. It is assumed that you have successfully performed the steps listed in Section 2.

## 3.a. frameworkMain
For each new shell navigate to the base directory of the repository and setup the environment via:

```
source scripts/setup_CMS_GEM.sh
```

The usage for the `frameworkMain` executable is:

```
./frameworkMain <PFN of Run Config File> <Verbose Mode true/false>
```

Here the physical file name (PFN) represents the full path+filename to the file in question. The configuration files, including the run config file, are described in Section 4.e.  The executable can analyze files produced either by `amoreSRS`, `amoreSRS_ZS`, or the `CMS_GEM_Analysis_Framework` (referred to as "the framework" henceforth).  Additionally the framework can unpack, decode, and reconstruct raw data recorded by the RD51 SRS system with FEC's running on the zero suppression firmware.  The unpacking, decoding, and reconstruction of RD51 SRS raw data is referred to as "reconstruction" henceforth.  Reconstruction of data recorded when not using the zero suppression firmware is not presently supported.

For a full explanation of the available configurations please consult Sections 4.e for a description, and Sections 3.a.i through 3.a.iii for examples. The contents and layout of the output files are described in Section 4.f.

Four example config files: 

- *mapping* config file, 
- *analysis* config file, 
- *reco* config file, and 
- *run* config file 

have been provided in the default repository.  A usage example is given as:

```
./frameworkMain config/configRun.cfg true
```

As a general rule each detector you are testing will usually have a series of raw files associated with it.  It is recommended each raw file should have a run number associated with it.  The set of run numbers for a given detector should be unique (i.e. two different detectors can have a run 1 but for a specific detector run 1 is unique).  The framework expects that you provide a run number, form `_RunX_` for `X` some integer, in each of the input files defined in your `config/configRun.cfg` (see Section 4.e.iii).

In addition to analyzing raw data to produce a framework output `ROOT` file it is also possible to analyze a series of framework outpt files to plot comparisons of any `TH1F` object stored in the output `ROOT` file.  For more details on running in this mode see Sections 3.a.iv and 4.e.iii.

In summary the running options of the frameowrk are:

1. Analysis, analyzing an input reconstructed SRS `TTree` file
2. Comparison, comparing one or more framework output `ROOT` files produced in otion #1,
3. Reconstruction, performing the reconstruction on an RD51 SRS input raw data file,
4. Combinded Reconstruction and Analysis (referred to as Combined), doing option 3 followed by option 1

Configuring for each of these options is described Secton 4.e.iii.  Option 1 can be executed in all modes: Grid, Series, or Re-run.  Option 3 can be executed in Grid or Series mode; but in series mode only one input RD51 SRS raw file should be supplied.  Option 4 can only be executed in Series mode, again the only one RD51 SRS raw file should be supplied. run modes while option #1 can be additionally exectued in the re-run mode.  See sections 3.a.i through 3.a.iv and 4.e.iii for further details.

### 3.a.i Helper Script - Run Mode: Grid (Analysis)
The script:

```
scripts/runMode_Grid.sh
```

is for running the framework with the analysis option with the lxplus batch submission system using the scheduler `bsub`. This script will setup the *run config file* and launch one job for each input `*dataTree.root` file found in the detector's file directory on `EOS`.  The expected synatx is:

```
runMode_Grid.sh <Detector Name> <Config File - Analysis> <Config File - Mapping> <Queue Name> <Output Dir>
```

Where: 

- **Detector Name** is the serial number of the detector, omitting slashes (i.e. "/") for example "GE11-X-S-CERN-0001" or "GE11-X-L-CERN-0015" be analyzed are located, 
- **Config File - Analysis** is the PFN of the input analysis config file, 
- **Config File - Mapping**  is the input mapping config file, 
- **Queue Name** is from the set {`8nm`, `1nh`, `8nh`, `1nd`} and is the submission queue on the [lxplus batch submmission system](https://cern.service-now.com/service-portal/article.do?n=KB0000470), and
- **Output Dir** is the physical file path (PFP) of the desired output data directory where output files will be moved once the job completes.

Additionally for each job a run config file (described in Section 4.e.iii.V), named `config/configRun_JobNoX.cfg` where `X` is the job number, will be created.  One script per job, named `scripts/submitFrameworkJob_JobNoX.sh`, will created; this script will be what the job executes when it runs.  Three directories will also be created, if they do not already exist, called `$GEM_BASE/stderr`, `$GEM_BASE/stdlog`, and `$GEM_BASE/stdout`.  Each of these directories will respectively store the stderr (`stderr/frameworkErr_JobNoX.txt`), stdlog (`stdlog/frameworkLog_JobNoX.txt`), and stdout (`stdout/frameworkOut_JobNoX.txt`) files produced for each job.  The stderr file for a job should be checked if a job fails to produce an output `TFile`.  The stdlog file for a job shows any output to terminal that would be created if the executable was run locally.  The stdout file shows a summary of the job prepared by the scheduler showing time taken, memory usage, and stderr file for the job.

After `scripts/runMode_Grid.sh` has submitted all your jobs it will dump some useful information for how to check running jobs and kill running jobs if necessary.  The `scripts/runMode_Grid.sh` will also show how to add all output `TFiles` together from command line using `hadd`.  After you have added all `TFiles` together you should run the framework over this summary `TFile` in re-run mode to perform the fitting and uniformity analysis.

After you have had a chance to investigate the created run config files, submission scripts, stderr, stdlog, and stdout files you can safely remove them by calling:

```
source scripts/cleanGridFiles.sh
```

For full example:

```
runMode_Grid.sh GE11-VII-L-CERN-0001 config/configAnalysis.cfg config/Mapping_GE11-VII-L.cfg 1nh $DATA_DIR
cd $DATA_DIR/GE11-VII-L-CERN-0001
hadd summaryFile_Ana.root *Ana.root
cd $GEM_BASE
source scripts/cleanGridFiles.sh
source scripts/runMode_Rerun.sh GE11-VII-L-CERN-0001 $DATA_DIR/GE11-VII-L-CERN-0001 config/configAnalysis.cfg config/Mapping_GE11-VII-L.cfg
```

NOTE: Modications to `config/configRun_Template_Grid.cfg` may lead to undefined behavior or failures; it is recommended to not modify the template config file.

If you are interested in merging only a subset of the output TFile's the `scripts/mergeSelectedRuns.sh` is provided to perform this task.  The expected syntax is:

```
source mergeSelectedRuns.sh <Output ROOT Filename w/Key Phrase> <Key Phrase> <Data File Directory> <Comma and/or Dash Delimited Run List>
```

Where: the **Output ROOT Filename w/Key Phrase** is the name of the merged ROOT file which includes a **Key Phrase** that the script will use string replacement to indicate the total number of events from the input run list (provided each input run has the field **YYkEvt** present in the filename); **Data Directory** is the PFP where the input data files to be merged are located; and **Comma and/or Dash Delimited Run List** is the list of input runs to be considered for merging.  Example:

```
source mergeSelectedRuns.sh GE11-VII-S-CERN-0002_Summary_Physics_RandTrig_XRay40kV99uA_580uA_YYkEvt_Ana.root YY $DATA_DIR/GE11-VII-S-CERN-0002 133,135,137-158
```

Here the key phrase is **YY** and runs 133, 135, and 137 through 158 will be merged together to make `GE11-VII-S-CERN-0002_Summary_Physics_RandTrig_XRay40kV99uA_580uA_YYkEvt_Ana.root` with `YY` replaced with the total event number indicated in the filenames.

### 3.a.ii Helper Script - Run Mode: Grid (Reconstruction)
The script:

```
scripts/runMode_Grid_Reco.sh
```

is for running the framework with the reconstruction option with the lxplus batch submission system using the scheduler `bsub`. This script will setup the run config file and launch one job for each input file in the data file directory below.  The expected synatx is:

```
runMode_Grid_Reco.sh <Detector Name> <Config File - Reco> <Config File - Mapping> <Queue Names> <Output Dir>
```

Where: the inputs are as in Section 3.a.i except that **Config File - Reco** is the PFN of the input reco config file.  The behavior of this script is identical to `runMode_Grid.sh` except that the input files found in the detector's file directory on `EOS` end with the expression `*.raw`. For furhter details consult Section 3.a.i.

After all your jobs have completed you are ready to process the created `*dataTree.root` files with the framework in the analysis option (see Sections 3.a & 4.e.iii).

Full example:
```
runMode_Grid_Reco.sh GE11-VII-L-CERN-0004 config/configReco.cfg config/Mapping_GE11-VII-L.cfg 1nh $DATA_DIR
cd $DATA_DIR/GE11-VII-L-CERN-0004
ls
cd $GEM_BASE
source scripts/cleanGridFiles.sh
source scripts/runMode_Series.sh GE11-VII-L-CERN-0004 $DATA_DIR/GE11-VII-L-CERN-0004 config/configAnalysis.cfg config/Mapping_GE11-VII-L.cfg
```

It is important to note that, unfortunately, the mapping file must be provided in two locations: 

- the Run Config file, and
- the Reco Config file.  

If you reconstruct one set of data with one mapping file, but then use a different mapping file to perform the analysis it is likely only empty histograms will be generated.

NOTE: Modications to `config/configRun_Template_Grid_Reco.cfg` may lead to undefined behavior or failures; it is recommended to not modify the template config file.

### 3.a.iii Helper Script - Run Mode: Rerun
The script:

```
scripts/runMode_Rerun.sh
```

is for running the framework with the analysis option over a previously produced framework output `TFile`. This script will setup the run config file to re-run over each input file found in the data file directory below. One output `TFile` will be produced for each input file.  The expected synatx is:

```
source scripts/runMode_Rerun.sh <Detector Name> <Data File Directory> <Config File - Analysis> <Config File - Mapping>
```

Where: **Detector Name** is the detector serial number; and **Data File Directory**, **Config File - Analysis**, and **Config File - Mapping** are as described in Section 3.a.i. After calling this script it is recommended to cross-check the created `config/configRun.cfg` file before executing the framework.  This will let you ensure the correct set of input files will be re-analyzed.

The script will only consider files that end in `*Ana.root`, the output of each will be `*NewAna.root`.

Full example:

```
source scripts/runMode_Rerun.sh GE11-VII-L-CERN-0001 $DATA_DIR/GE11-VII-L-CERN-0001 config/configAnalysis.cfg config/Mapping_GE11-VII-L.cfg
./frameworkMain config/configRun.cfg true
```

NOTE: Modications to `config/configRun_Template_Rerun.cfg` may lead to undefined behavior or failures; it is recommended to not modify the template config file.

### 3.a.iv Helper Script - Run Mode: Series
The script:

```
scripts/runMode_Series.sh
```

is for running the framework with the analysis option over a set of input files in series, i.e. one after the other, and creating a single output `TFile`.  The expected syntax is:

```
source runMode_Series.sh <Detector Name> <Data File Directory> <Config File - Analysis> <Config File - Mapping> <Output Data Filename>
```

Where: **Detector Name**, **Data File Directory**, **Config File - Analysis**, and **Config File - Mapping** are as described in Sections 3.a.i and 3.a.ii; and **Output Data Filename** is the desired name of the output `TFile` to be created. After calling this script it is recommended to cross-check the created `config/configRun.cfg` file before executing the framework.  This will let you ensure the correct set of input files will be analyzed.

Full example:

```
source runMode_Series.sh GE11-VII-L-CERN-0001 $DATA_DIR/GE11-VII-L-CERN-0001 config/configAnalysis.cfg config/Mapping_GE11-VII-L.cfg GE11-VII-L-CERN-0001_FrameworkAna.root
./frameworkMain config/configRun.cfg true
```

Right now no helper script exists for running the reconstruction or combined options in the series mode. Presently these modes can only be run over a single input file.  Manual configuration of the run config file is required (See Section 4.e.iii).

NOTE: Modications to `config/configRun_Template_Series.cfg` may lead to undefined behavior or failures; it is recommended to not modify the template config file.

### 3.a.v  Helper Script - Run Mode: Comparison
The script:

```
scripts/runMode_Comparison.sh
```

is for running the framework with the analysis option over a set of framework output files and creating a single output `TFile` containing a `TCanvas` with a set of `TH1F` objects drawn on it (and also stored in the ROOT file).  The expected syntax is:

```
source runMode_Comparison.sh <Data File Directory> <Output Data Filename> <Obs Name> <iEta,iPhi,iSlice> <Identifier>
```

Where: 

- **Data File Directory** is as above; 
- **Output Data Filename** is the name of the output `TFile`; 
- **Obs Name** is a regular expression found in the `TName` of the `TH1F` objects you wish to compare from your set of input files; 
- **iEta,iPhi,iSlice** is a comma separated triplet of the `(iEta,iPhi,iSlice)` coordinate within the detector; and 
- **Identifier** is a regular expression contained in the filenames of each of your input files found in the **Data File Directory**. 

When giving the **Obs Name** this is the `ObservableNameX` referred to in Section 4.f.i. To familiarize yourself with the possible inputs it is suggestion to run the framework in one of the above outputs and study the produced `TFile`.

For the **iEta,iPhi,iSlice** field you must always enter a set of three integers.  However, you can access observables created at either the `Summary`, `SectorEta`, `SectorPhi`, or `Slice` level (Again see Section 4.f.i) based on the input that is given.  The following table shows how to access observables at each level:

Obs Level | iEta | iPhi | iSlice
--------- | ---- | ---- | ------
Summary | -1 | -1 | -1
SectorEta | iEta | -1 | -1
SectorPhi | iEta | iPhi | -1
Slice | iEta | iPhi | iSlice

As you can see by placing `-1` in the relevant coordinate point you can select the level you are interested in. 

Since this is a different run mode, to prevent any previous Run Config file from being over-written, this script will produce a Run Config file called:

```
config/configComp.cfg
```

this should help you distinguish your different configurations. After calling this script it is recommended to cross-check the created `config/configComp.cfg` file before executing the framework.  This will let you ensure the correct set of input files will be analyzed.

Full example:

```
source runMode_Comparison.sh data/clustSelStudy/GE11-VII-L-CERN-0002 GE11-VII-L-CERN-0002_ClustSize_Comparison.root clustADC 4,2,-1 ClustSize
```

In this example the contents of my Data File Directory are:

```
% ls data/clustSelStudy/GE11-VII-L-CERN-0002
GE11-VII-L-CERN-0002_Summary_Physics_RandTrig_AgXRay40kV100uA_580uA_15004kEvt_ClustTime6to27_ClustSize1to20_Ana.root
GE11-VII-L-CERN-0002_Summary_Physics_RandTrig_AgXRay40kV100uA_580uA_15004kEvt_ClustTime6to27_ClustSize1_Ana.root
GE11-VII-L-CERN-0002_Summary_Physics_RandTrig_AgXRay40kV100uA_580uA_15004kEvt_ClustTime6to27_ClustSize2_Ana.root
GE11-VII-L-CERN-0002_Summary_Physics_RandTrig_AgXRay40kV100uA_580uA_15004kEvt_ClustTime6to27_ClustSize3_Ana.root
GE11-VII-L-CERN-0002_Summary_Physics_RandTrig_AgXRay40kV100uA_580uA_15004kEvt_ClustTime6to27_ClustSize4_Ana.root
GE11-VII-L-CERN-0002_Summary_Physics_RandTrig_AgXRay40kV100uA_580uA_15004kEvt_ClustTime6to27_ClustSize5_Ana.root
GE11-VII-L-CERN-0002_Summary_Physics_RandTrig_AgXRay40kV100uA_580uA_15004kEvt_ClustTime6to27_ClustSize6_Ana.root
```

you can see the **Identifier** regular expression is **ClustSize** which is contained in each filename as `ClustSizeX` for `X` = {`1to20`,`1`,`2`,...,`5`,`6`}.  And the observable I will be comparing across these files is the clustADC `TH1F` found in the `(iEta,iPhi) = (4,2)` sector.  Since `iSlice = -1` the full `SectorPhi` is considered.

Right now this mode is somewhat primitive.  If you enter an **Obs Name** and `(iEta,iPhi,iSlice)` combination that does not exist it will crash and seg-fault.  If this occurs please double-check your input file. Note that there is a difference between `clustADC` and `ClustADC` with the former working, and the latter causing a seg fault (i.e. the "c" is not capitalized).

NOTE: Modications to `config/configComp_Template.cfg` may lead to undefined behavior or failures; it is recommended to not modify the template config file.

## 3.b. genericPlotter
As with `frameworkMain` for each new shell navigate to the base directory of the repository and setup the environment via:

```
source scripts/setup_CMS_GEM.sh
```

The usage for the `genericPlotter` executable is:

```
./genericPlotter <PFN of Plot Config File> <Verbose Mode true/false>
```

The **Plot Config** file is described in Section 4.e.iv.  The executable can take in comma separted data or `TObjects` from an input `TFile`.  Presently `TGraph`, `TGraph2D`, `TGraphErrors`, `TH1F`, and `TH2F` objects are supported.

The `genericPlotter` will create a canvas following the official [CMS style guide](https://ghm.web.cern.ch/ghm/plots/) for figures defined by the [CMS Collaboration](https://twiki.cern.ch/twiki/bin/view/CMS/Internal/PubGuidelines#Figures_and_tables). Developers should periodically check for updates to these guidelines.  This executable has been adaptered from the `tdrstyle.C`, `CMS_lumi.h`, and `CMS_lumi.C` scripts available from the first link. Bob Brown, Gautier Hamel de Monchenault, and Dino Ferencek are the authors/contributors to these three scripts at the time of their adaptation.

For a full explanation of the available configurations please consult Section 4.e.iv.  The contents and layout of the output files are described in Section 4.f.iii.

Five example plot config files for plotting: 

- `TGraph`, 
- `TGraph2D`, 
- `TGraphErrors`, 
- `TH1F`, and
- `TH2F`

objects have been provided in the default repository.  A usage example is given as:

```
./genericPlotter config/configPlot_Graph.cfg true
```

As a general rule the style defined by `genericPlotter` may not persist in the created `TObjects` once they have been saved in the output `TFile`.  Additionally your `rootlogon.C` script may define a different style than the used in the official CMS guide.  As a result it is strongly suggested to relying on the output image files created by `genericPlotter` and not the output `ROOT` file.

### 3.b.i  Helper Script - Make All Plots
The helper script:

```
scripts/makeAllPlots.sh
```

Has been created for you to easily make plots for all plot config files that are defined in a given directory.  This can help automatize the plot making procedure. The expected syntax is:

```
source scripts/makeAllPlots.sh <Plot Config File Directory>
```

Where **Plot Config File Directory** is the directory where your plot config files are found.  Note your plot config files must have the `*.cfg` extension to be recognized. An example call is given as:

```
source scripts/makeAllPlots.sh figures/ResponseUniformityMaps
```

this will then execute genericPlotter taking each `*.cfg` file in the `figures/ResponseUniformityMaps` directory.

## 3.b. Python Scripts
A set of python analysis tools has been added to assist the user in further analysis of data created with the Framework.  The mathematical framework for the following sections is described [here](https://indico.cern.ch/event/631320/contributions/2552041/attachments/1444163/2224433/BDorney_SliceTest_HV_Settings.pdf). This may be helpful in attempting to understand the results produced by the python tools described below.

### 3.c.i  Analysis Suite - Gain Map
### --------------------------------------------------------
This tool takes results from the effective gain calibration (`QC5_Eff_Gain`) and response uniformity (`QC5_Resp_Uni`) measurements to determine the gain at every point across the detector.  As with `frameworkMain` for each new shell navigate to the base directory of the repository and setup the environment via:

```
source scripts/setup_CMS_GEM.sh
```

The usage for the `computeGainMap.py` script is:

```
    python2.7 python/computeGainMap.py --file=<Framework Output File> --gp0=<Gain P0> --gp0Err=<Gain P0Err> --gp1=<Gain P1> --gp1Err=<Gain P1Err> --name=<Detector Name> --hvPoint=<HV Val> --hvlist=<HV List> --fileMap=<Config File - Mapping>
```

Where: 

- **Framework Output File** is the PFN of the analyzed framework output file produced from the `QC5_Resp_Uni` measurement;
- **Gain P0**, **Gain P0Err**, **Gain P1**, and **Gain P1Err** are values of the parameters, and their errors, of a fit to the `QC5_Eff_Gain` data where the equation of fit is described as `G(x) = exp([0]*x+[1])` with `x` some `hvPt` observable (either drift voltage or divider current);
- **Detector Name** is the exact string assigned to the `Detector_Name` field in the run config file you supplied when creating the given Framework Output File; 
- **HV Val** is the value of the `hvPt` observable (either drift voltage or divider current) for which the `QC5_Resp_Uni` measurement was carried out at (note: this must match with how the gain curve is parameterized); 
- **HV List** is a comma separated list of HV points you would like the gain map calculated at (again this must match how gain curve is parameterized); and 
- **Config File - Mapping** is the input mapping config file used to produce the analyzed framework output file.

An example call is given as:

```
python2.7 python/computeGainMap.py --file=FrameworkOutput.root --gp0=3.49545e-02 --gp0Err=1.98035e-04 --gp1=-1.40236e+01 --gp1Err=1.28383e-01 --name=GE11-VII-L-CERN-0002 --hvPoint=580 --hvlist=600,625,650,660,670,680,690,700,710,720,730 --fileMap=config/Mapping_GE11-VII-L.cfg
```

Here the gain curve was parameterized in terms of divider current and as a result the `hvPoint` and `hvlist` fields are also given in terms of divider current. If you would like to analyze multiple files simultaneously, or prefer to not type everything in on command line, the helper script:

```
scripts/makeGainMaps.sh
```

has been created to assist you.  The expected synatx is:

```
source makeGainMaps.sh <Summary File>
```

where **Summary File** is a tab delimited data file where each line has the command line arguments you would pass for a single single detector.  Specifically the order is expected as:

```
file	gp0	gp0Err	gp1	gp1Err	name	hvPoint	hvlist  fileMap
```

Where these fields are described in the following table:

Field | Description
----- | -----------
file | physical filename of the Framework output file
gp0 | Value of `p0` in `G(x) = exp(p0*x+p1)`
gp0Err | Error on `p0` in `G(x) = exp(p0*x+p1)` 
gp1 | Value of `p1` in `G(x) = exp(p0*x+p1)`
gp1Err | Error on `p1` in `G(x) = exp(p0*x+p1)` 
name | String assigned to `Detector_Name` field in the run config file used to make the Framework output file
hvPoint | HV value at which the original `QC5_Resp_Uni` measurement was performed at
hvlist | comma separated list of hv points for which the gain map should be calculated at
fileMap | physical filename of the mapping config file used to generate the Framework output file

in the above table `x` in `G(x) = exp(p0*x+p1)` is either in terms divider current or `V_Drift`. For each entry in the **Summary File** the `python/computeGainMap.py` script will be called and generate the appropriate output file.

### 3.c.ii Analysis Suite - Efficiency Predictions
This tool takes results from the effective gain calibration (`QC5_Eff_Gain`), response uniformity (`QC5_Resp_Uni`) measurements, MIP cluster charge measurements, and MIP cluster size measurements to determine the gain and efficiency at every point across the detector.  As with `frameworkMain` for each new shell navigate to the base directory of the repository and setup the environment via:

```
source scripts/setup_CMS_GEM.sh
```

The usage for the `computeEffCurves.py` script is:

```
python2.7 python/computeEffCurves.py  --file=<Efficiency Predictor Config File> --debug=<True/False>
```

Where the **Efficiency Predictor Config File** is described in Section 4.e.v and the debug flag is a True/False value which will print more verbose information to the terminal. An example call is given as:

```
python2.7 python/computeEffCurves.py --file=config/configEffPredictor.cfg --deubg=True
```

# 4. Documentation
This section describes the `C++` contents of the repository, the expected inputs, and the produced outputs. Developers should have a firm grasp of this entire section; users need only be concerned with sections 4.e and 4.f.  However users may be interested in understanding what `TObjects` are created/stored in the output `ROOT` file and may wish to browse the sections below.

## 4.a. Namespaces
This repository has declared the following namespaces: `Luminosity`, `Plotter`, `Timing`, `Uniformity`. The last three of these namespaces reside within the `QualityControl` namespace. The `Luminosity` and `Plotter` namespaces include tools necessary for creating plots conforming to the offical CMS Style Guide.

The `Timing` namespace includes several operators, types, and utility functions that were developed in `CMS_GEM_TB_Timing`; the contents of the `Timing` namespace offer substantial utility and "quality of life features."

The `Uniformity` namespace contains the majority of source code for analyzing response uniformity measurements performed for GE1/1 detectors; and hopefully GE2/1 & ME0 detectors in the future.

## 4.b. C++ Class Map
Inheritance relations:

    FrameworkBase
        |
        |--->AnalyzeResponseUniformity
        |       |
        |       |--->AnalyzeResponseUniformityClusters
        |       |--->AnalyzerResponseUniformityHits
        |       |--->Visualizer
        |               |
        |               |--->VisualizeComparison
        |               |--->VisualizeUniformity
        |
        |--->Interface
        |       |
        |       |--->InterfaceAnalysis
        |       |--->InterfaceReco  (Skeleton, not implemented yet)
        |       |--->InterfaceRun   (Depreciated)
        |
        |--->Selector
                |
                |--->SelectorCluster
                |--->SelectorHit

    ParameterLoader
        |
        |--->ParameterLoaderAnalysis
        |--->ParameterLoaderDetector
        |--->ParameterLoaderFit
        |--->ParameterLoaderPlotter
        |--->ParameterLoaderRun

    PlotterGeneric
        |
        |--->PlotterGraph
        |--->PlotterGraph2D
        |--->PlotterGraphErrors
        |--->PlotterHisto
        |--->PlotterHisto2D

    ReadoutSector
        |
        |--->ReadoutSectorEta
        |--->ReadoutSectorPhi

The following classes have no children presently:

- DetectorMPGD
- SRSAPVEvent
- SRSCluster
- SRSConfiguration
- SRSEventBuilder
- SRSFECDecoder
- SRSHit
- SRSMain
- SRSMapping
- SRSOutputROOT

Friendship relations:

- AnalyzeResponseUniformity, AnalyzeResponseUniformityClusters, AnalyzeResponseUniformityHits,
- InterfaceAnalysis, and ParameterLoaderDetector are all friend classes to DetectorMPGD.

Interactions:

- Classes ParameterLoaderDetector, those inheriting from Selector, and those inheriting from
- AnalyzeResponseUniformity all act on an object of DetectorMPGD.
 
- The ParameterLoaderAnalysis class interacts with objects who inherit from Selector and
- AnalyzResponseUniformity classes.

The table below gives a brief description of some of the major classes used in the Framework:

Class | Description
----- | -----------
InterfaceAnalysis |  interface between `main()` and the analysis portion of the framework; runs the analysis for loaded case.
ParameterLoaderAnalysis | sets up the user specified analysis; this info is passed separately to `Selector` & `AnalyzeResponseUniformity` classes (and their inherited classes).
ParameterLoaderDetector | creates a `DetectorMPGD` object
ParameterLoaderFit | loads necessary information for fits
ParameterLoaderPlotter | loads necessary information for making a plot conforming to the CMS Style Guide
ParameterLoaderRun | sets up the run configuration, the files to be analyzed, and what analysis stages (e.g. hits, clusters, fitting, etc...) to be exectued.
PlotterGeneric | And it's inherited classes create a `TCanvas` with one or more `TObjects` drawn on it such that it conforms to the CMS Style Guide
ReadoutSector | A single readout sector, used by `DetectorMPGD` to track distributions in a certain portion of the detector
ReadoutSectorEta | As `ReadoutSector`, but for an iEta row inside the detector, stores the detector's `ReadoutSectorPhi` objects
ReadoutSectorPhi | As `ReadoutSectorEta`, but for an iPhi sector within an iEta row, stores the detector's hits, clusters, and slices
SelectorCluster | Assigned an input `DetectorMPGD` object the opens an input root file and performs the cluster selection; selected clusters are stored based on their location in the `DetectorMPGD` object.
SelectorHit | As `SelectorCluster` but for hits (e.g. single strips).
AnalyzeResponseUniformityCluster | Acts on a `DetectorMPGD` object that has stored clusters and performs the user requested analysis
AnalyzeResponseUniformityHit | As `AnalyzeResponseUniformityCluster` but for hits.
VisualizeUniformity | Takes the raw plots produced by `AnalyzeResponseUniformity` and presents them in a user friendly manner.
VisualizeComparison | Compares `TH1F` objects from different Framework output files and makes simple comparison plots

An example process-flow of what `frameworkMain` does in a given execution is shown as:

    frameworkMain
        |
        |->ParameterLoaderRun loads run parameters from run config file
        |
        |->Run Option Analysis
        |       |
        |       |->ParameterLoaderDetector creates a DetectorMPGD Object
        |       |
        |       |->ParameterLoaderAnalysis & ParameterLoaderFit load analysis parameters from analysis config file
        |       |
        |       |->InterfaceAnalysis is given run & analyiss setup structs, and DetectorMPGD object
        |               |
        |               |->Reconstructed Tree File Input?
        |               |       |
        |               |       |->SelectorHit performs hit selection and stores selected hits in DetectorMPGD object
        |               |       |
        |               |       |->AnalyzeResponseUniformityHits makes distributions from selected hits
        |               |       |
        |               |       |->SelectorCluster does the same for clusters
        |               |       |
        |               |       |->AnalyzeResponseUniformityClusters makes distributions from selected clusters
        |               |       |
        |               |       |->After all input files are processed AnalyzeResponseUniformityClusters makes & fits slice distributions
        |               |       |
        |               |       |->VisualizeUniformity creates summary plots
        |               |
        |               |->Framework output file as input?
        |                       |
        |                       |->AnalyzeResponseUniformityClusters loads all previously created TObjects from file
        |                       |
        |                       |->AnalyzeResponseUniformityClusters then makes then fits slice distributions
        |                       |
        |                       |->VisualizeUniformity creates summary plots
        |
        |->Run Option Comparison
        |       |
        |       |->VisualizeComparison collects all TH1F objects for comparison from all input files
        |       |
        |       |->VisualizeComparison collected TH1F objects and sets up a legend for identification
        |
        |->Run Option Reconstruction
        |       |
        |       |->SRSMain produces a Tree file from a RD51 SRS raw file (BLACK BOX!!!)
        |
        |->Run Option Combined
                |
                |->Run Option Reconstruction is executed
                |
                |->Run Option Analysis is executed

The following sections give further documentation about the `C++` classes in the Framework.

### 4.b.i. FrameworkBase
Defined in `include/FrameworkBase.h` Non-inherited member attributes shown below.

```
Public Constructors
    FrameworkBase();

Public Member Functions
    virtual Uniformity::DetectorMPGD getDetector();
    virtual int getRunNum();

    virtual void setAnalysisParameters(Uniformity::AnalysisSetupUniformity inputSetup);
    virtual void setDetector(Uniformity::DetectorMPGD & inputDet);
    virtual void setRunNum(int iInput);
    virtual void setRunParameters(Uniformity::RunSetup inputSetup);
    virtual void setVerboseMode(bool bInput);

Protected Attributes
    bool bVerboseMode;
    int iNum_Run;

    QualityControl::Uniformity::RunSetup rSetup;
    QualityControl::Uniformity::AnalysisSetupUniformity aSetup;
    QualityControl::Uniformity::DetectorMPGD detMPGD;
```

### 4.b.ii. Analyzers & Visualization
Coming "soon"

#### 4.b.ii.I AnalyzeResponseUniformity
Defined in `include/AnalyzeResponseUniformity.h` Inherits from `FrameworkBase`. Non-inherited member attributes shown below.

```
Public Types
    typedef exprtk::symbol_table<float> symbol_table_t;
    typedef exprtk::expression<float> expression_t;
    typedef exprtk::parser<float> parser_t;

Public Constructors
    AnalyzeResponseUniformity()
    AnalyzeResponseUniformity(Uniformity::AnalysisSetupUniformity inputSetup, Uniformity::DetectorMPGD & inputDet);

Protected Member Functions
    void calcStatistics(Uniformity::SummaryStatistics &inputStatObs, multiset<float> &mset_fInputObs, std::string strObsName);
    bool isQualityFit(std::shared_ptr<TF1> fitInput);
    bool isQualityFit(std::shared_ptr<TF1> fitInput, int iPar);
    TF1 getFit(int iEta, int iPhi, int iSlice, Timing::HistoSetup & setupHisto, shared_ptr<TH1F> hInput, TSpectrum &specInput );
    float getParsedInput(string &strInputExp, shared_ptr<TH1F> hInput, TSpectrum &specInput);
    TGraphErrors getGraph(int iEta, int iPhi, Timing::HistoSetup &setupHisto);
    TH1F getHistogram(int iEta, int iPhi, Timing::HistoSetup &setupHisto);
    TH2F getHistogram2D(int iEta, int iPhi, Timing::HistoSetup &setupHisto_X, Timing::HistoSetup &setupHisto_Y);
    string getNameByIndex(int iEta, int iPhi, int iSlice, string & strInputPrefix, string & strInputName);
    string getNameByIndex(int iEta, int iPhi, int iSlice, const string & strInputPrefix, const string & strInputName);
    float getParam( shared_ptr<TF1> fitInput, Timing::HistoSetup & setupHisto, string strParam );
    float getParamError( shared_ptr<TF1> fitInput, Timing::HistoSetup & setupHisto, string strParam );
    float getValByKeyword(string strInputKeyword, shared_ptr<TH1F> hInput, TSpectrum &specInput);

Protected Attributes
    string strAnalysisName;
    vector<string> vec_strSupportedKeywords;
```

#### 4.b.ii.II AnalyzeResponseUniformityClusters
Defined in `include/AnalyzeResponseUniformityClusters.h`. Inherits from `AnalyzeResponseUniformity`. Non-inherited member attributes shown below.

```
Public Constructors
    AnalyzeResponseUniformityClusters()
    AnalyzeResponseUniformityClusters(Uniformity::AnalysisSetupUniformity inputSetup, Uniformity::DetectorMPGD & inputDet);

Public Member Functions
    virtual void fillHistos(DetectorMPGD & inputDet)
    virtual void fitHistos(DetectorMPGD & inputDet)

    virtual void initGraphsClusters(DetectorMPGD & inputDet);
    virtual void initHistosClusters(DetectorMPGD & inputDet);
    virtual void initHistosClustersByRun(int iInputRunNo, DetectorMPGD & inputDet);

    virtual void loadHistosFromFile(std::string & strInputMappingFileName, std::string & strInputROOTFileName);
    virtual void loadHistosFromFile(std::string & strInputMappingFileName, TFile * file_InputRootFile);

    void storeHistos(std::string & strOutputROOTFileName, std::string strOption, DetectorMPGD & inputDet);
    void storeHistos(TFile * file_InputRootFile, DetectorMPGD & inputDet);

    void storeFits(std::string & strOutputROOTFileName, std::string strOption, DetectorMPGD & inputDet);
    void storeFits(TFile * file_InputRootFile, DetectorMPGD & inputDet);
```

#### 4.b.ii.III AnalyzeResponseUniformityHits
Defined in `include/AnalyzeResponseUniformityHits.h`. Inherits from `AnalyzeResponseUniformity`. Non-inherited member attributes shown below.

```
Public Constructors
    AnalyzeResponseUniformityHits();
    AnalyzeResponseUniformityHits(Uniformity::AnalysisSetupUniformity inputSetup);

Public Member Functions
    virtual void fillHistos(DetectorMPGD & inputDet);
    virtual void findDeadStrips(DetectorMPGD & inputDet, std::string & strOutputTextFileName);
    virtual void fitHistos(DetectorMPGD & inputDet);

    virtual void initHistosHits(DetectorMPGD & inputDet);

    virtual void loadHistosFromFile(std::string & strInputMappingFileName, std::string & strInputROOTFileName);

    void storeHistos(std::string & strOutputROOTFileName, std::string strOption, DetectorMPGD & inputDet);
    void storeHistos(TFile * file_InputRootFile, DetectorMPGD & inputDet);

    void storeFits(std::string & strOutputROOTFileName, std::string strOption, DetectorMPGD & inputDet);
```

#### 4.b.ii.IV Visualizer
Defined in `include/Visualizer.h`. Inherits from `AnalyzeResponseUniformity`. Non-inherited member attributes shown below.

```
Public Constructors
    Visualizer()

Public Member Functions
    virtual void setAutoSaveCanvas(bool bInput){ m_bSaveCanvases = bInput; return; };

Protected Member Functions
    virtual void save2png(TCanvas & inputCanvas);
    virtual TPad *getPadEta(int iEta, int iNumEta);
    virtual TPad *getPadPhi(int iEta, int iNumEta, int iPhi, int iNumPhi);

Protected Attributes
    bool m_bSaveCanvases;
```

#### 4.b.ii.V VisualizeComparison
Defined in `include/VisualizeComparison.h`. Inherits from `Visualizer`. Non-inherited member attributes shown below.

```
Public Constructors
    VisualizeComparison();

Public Member Functions
    virtual void storeCanvasComparisonHisto()
    virtual void storeCanvasComparisonHisto(std::string strOutputROOTFileName, std::string strOption, std::string strObsName)
    virtual void storeCanvasComparisonHisto(TFile * file_InputRootFile, std::string strObsName)

    virtual void setDrawOption(std::string strInput)

    virtual void setIdentifier(std::string strInput)

    virtual void setInputFiles(std::vector<std::string> vec_strInput)

    virtual void setNormalize(bool bInput)

    virtual void setPosEta(int iInput);
    virtual void setPosEtaPhi(int iInputEta, int iInputPhi)
    virtual void setPosFull(int iInputEta, int iInputPhi, int iInputSlice)
    virtual void setPosPhi(int iInput)
    virtual void setPosSlice(int iInput)

    virtual void setRunParameters(Uniformity::RunSetup inputSetup)  //Intentially overrides FrameworkBase::setRunParameters(Uniformity::RunSetup inputSetup)

Private Methods
    virtual std::shared_ptr<TH1F> getObsHisto(TFile * file_InputRootFile, std::string strObsName)
    virtual std::map<std::string, std::shared_ptr<TH1F> > getObsHistoMap(std::string strObsName)

Private Attributes
    bool m_bNormalize

    int m_iEta
    int m_iPhi
    int m_iSlice

    std::string m_strIdent
    std::string m_strDrawOption

    std::vector<std::string> m_vec_strFileList;
```

#### 4.b.ii.VI VisualizeUniformity
Defined in `include/VisualizeUniformity.h`. Inherits from `Visualizer`. Non-inherited member attributes shown below.

```
Public Constructors
    VisualizeUniformity();
    VisualizeUniformity(Uniformity::AnalysisSetupUniformity inputSetup, Uniformity::DetectorMPGD inputDet);

Public Member Functions
    virtual void makeAndStoreCanvasHisto2D(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption);
    virtual void makeAndStoreCanvasHisto2D(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption);

    virtual void storeCanvasData(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption);
    virtual void storeCanvasData(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption);

    virtual void storeCanvasFits(std::string & strOutputROOTFileName, std::string strOption, std::string strDrawOption);
    virtual void storeCanvasFits(TFile * file_InputRootFile, std::string strDrawOption);

    virtual void storeCanvasGraph(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);
    virtual void storeCanvasGraph(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);

    virtual void storeCanvasGraph2D(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bNormalize);
    virtual void storeCanvasGraph2D(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bNormalize);

    virtual void storeCanvasHisto(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);
    virtual void storeCanvasHisto(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);

    virtual void storeCanvasHistoSegmented(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);
    virtual void storeCanvasHistoSegmented(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);

    virtual void storeCanvasHisto2DHistorySegmented(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bIsEta);
    virtual void storeCanvasHisto2DHistorySegmented(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bIsEta);

    virtual void storeListOfCanvasesGraph(std::string & strOutputROOTFileName, std::string strOption, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);
    virtual void storeListOfCanvasesGraph(TFile * file_InputRootFile, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);

    virtual void storeListOfCanvasesHisto(std::string & strOutputROOTFileName, std::string strOption, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);
    virtual void storeListOfCanvasesHisto(TFile * file_InputRootFile, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);

    virtual void storeListOfCanvasesHistoSegmented(std::string & strOutputROOTFileName, std::string strOption, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);
    virtual void storeListOfCanvasesHistoSegmented(TFile * file_InputRootFile, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);

Private Methods
    virtual TCanvas *getCanvasSliceFit(Uniformity::SectorSlice & inputSlice, int iEta, int iPhi, int iSlice, bool bDataOverFit);

    virtual Uniformity::SummaryStatistics getObsData(std::string strObsName);
    virtual std::shared_ptr<TGraphErrors> getObsGraph(std::string strObsName, Uniformity::ReadoutSectorEta &inputEta);
    virtual std::shared_ptr<TH1F> getObsHisto(std::string strObsName, Uniformity::ReadoutSector &inputSector);

    virtual std::map<int, std::shared_ptr<TH2F> > getMapObsHisto2D(std::string strObsName, Uniformity::ReadoutSector &inputSector);

    virtual std::shared_ptr<TH2F> getSummarizedRunHistoryHisto2D(std::map<int, std::shared_ptr<TH2F> > inputMapHisto2D, int iEta, int iPhi );
```

### 4.b.iii. Interfaces

#### 4.b.iii.I Interface
Coming "soon"

#### 4.b.iii.II InterfaceAnalysis
Coming "soon"

#### 4.b.iii.III InterfaceReco
Skeleton class, not yet implemented

#### 4.b.iii.IV InterfaceRun
Depreciated class, no longer used but kept in Framework.

### 4.b.iv. Selectors
These classes are for performing the cluster/event/hit selection specified by the user.

#### 4.b.iv.I Selector
Coming "soon"

#### 4.b.iv.II SelectorCluster
Coming "soon"

#### 4.b.iv.III SelectorHit
Coming "soon"

### 4.b.v. Loaders
These classes are loading user specified parameters at runtime.

#### 4.b.v.I ParameterLoaderAnalysis
Coming "soon"

#### 4.b.v.II ParameterLoaderDetector
Coming "soon"

#### 4.b.v.III ParameterLoaderFit
Coming "soon"

#### 4.b.v.IV ParameterLoaderPlotter
Coming "soon"

#### 4.b.v.V ParameterLoaderRun
Coming "soon"

### 4.b.vi. Plotters
Coming "soon"

#### 4.b.vi.I PlotterGeneric
Coming "soon"

#### 4.b.vi.II PlotterGraph
Coming "soon"

#### 4.b.vi.III PlotterGraph2D
Coming "soon"

#### 4.b.vi.IV PlotterGraphErrors
Coming "soon"

#### 4.b.vi.V PlotterHisto
Coming "soon"

#### 4.b.vi.VI PlotterHisto2D
Coming "soon"

### 4.b.vii. Readouts
Coming "soon"

#### 4.b.vii.I ReadoutSector
More coming "soon". Defined `in include/ReadoutSector.h`. Non-inherited member attributes shown below.

```
Public Member Functions
    ReadoutSector();
    ReadoutSector(const ReadoutSector & other);

Public Attributs
    float fWidth;

    Uniformity::HistosPhysObj clustHistos;
    Uniformity::HistosPhysObj hitHistos;
```

These data members are described in the table below:

Data Member | Description
----------- | -----------
`fWidth` | Width of sector in mm along the x-dir at the sector's y-location
`clustHistos` | Tracks observables for clusters
`hitHistos` | Tracls observables for hits

#### 4.b.vii.II ReadoutSectorEta
The `Uniformity::ReadoutSectorEta` represents one iEta row of a detector. Each instance of a `Uniformity::ReadoutSectorEta` will store `nbConnect` objects of a `Uniformity::ReadoutSectorPhi` class where `nbConnect` is a field found in the *mapping file* defining the number of readout conncetors per iEta row. Each object of a `Uniformity::ReadoutSectorPhi` will store `Uniformity::AnalysisSetupUniformity::iUniformityGranularity` number of `Uniformity::SectorSlice` struct objects.  An object of a `Uniformity::DetectorMPGD` class will store a number of objects of `Uniformity::ReadoutSectorEta` as defined in the *mapping file* (e.g. number of **DET** rows).

Defined in `include/ReadoutSectorEta.h`. Inherits from `ReadoutSector`. Non-inherited member attributes shown below.

```
Public Member Functions
    ReadoutSectorEta();
    ReadoutSectorEta(const ReadoutSectorEta & other);

Public Attributs
    float fPos_Y;

    std::map<int, Uniformity::ReadoutSectorPhi> map_sectorsPhi;

    std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_NormChi2;
    std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_PkPos;
    std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_PkRes;
    std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_Failures;

    std::shared_ptr<TGraphErrors> gEta_ClustADC_Spec_NumPks;
    std::shared_ptr<TGraphErrors> gEta_ClustADC_Spec_PkPos;

Operators
    ReadoutSectorEta & operator=(const ReadoutSectorEta & other);
```

Each of these items are described in the table below:

Data Member | Description
----------- | -----------
`fPos_Y` | Vertical Midpoint, in mm, of iEta row from wide base of trapezoid
`map_sectorsPhi` | Container storing nbConnect instances of `ReadoutSectorPhi` objects
`gEta_ClustADC_Fit_NormChi2` | NormChi2 of fits from all `SectorSlice::hSlice_ClustADC`
`gEta_ClustADC_Fit_PkPos` | ADC spec peak position from fits of all `SectorSlice::hSlice_ClustADC`
`gEta_ClustADC_Fit_PkRes` | ADC spec peak resolution.  Resolution is taken as (FWHM / Mean).  The FWHM and mean are taken from the fit results from fits of all `SectorSlice::hSlice_ClustADC`.  See Section 4.e.ii.IV for more details.
`gEta_ClustADC_Fit_Failures` | As `SectorEta::gEta_ClustADC_Fit_PkPos` but for when the minimizer did not succeed in finding a minima
`gEta_ClustADC_Spec_NumPks` | Number of peaks found in the `SectorSlice::hSlice_ClustADC` histogram; based on `TSpectrum::Search()` and `TSpectrum::GetNPeaks()`
`gEta_ClustADC_Spec_PkPos` | As `SectorEta::gEta_ClustADC_Fit_PkPos` but from `TSpectrum::Search()` and `TSpectrum::GetPositionX()` instead of fitting

The copy constructor and one overloaded assignment operator perform a deep copy of the `std::shared_ptr` objects above.

#### 4.b.vii.III ReadoutSectorPhi
Defined in `include/ReadoutSectorPhi.h`. Inherits from `ReadoutSector`. Non-inherited member attributes shown below.

```
Public Member Functions
    ReadoutSectorPhi();
    ReadoutSectorPhi(const ReadoutSectorPhi & other);

Public Attributs
    float fNFitSuccess;
    float fPos_Xlow;
    float fPos_Xhigh;

    int iStripNum_Min;
    int iStripNum_Max;

    std::map<int, Uniformity::SectorSlice> map_slices;

    std::multimap<int, Uniformity::Cluster> map_clusters;
    std::multimap<int, Uniformity::Hit> map_hits;

Operators
    ReadoutSectorPhi & operator=(const ReadoutSectorPhi & other);
```

Each of these items are described in the table below:

Data Member | Description
----------- | -----------
`fNFitSuccess` | Tracks the number of slices that were successfully fit
`fPos_Xlow` | X lower boundary of iPhi sector, in mm, at `ReadoutSectorEta::fPos_Y`
`fPos_Xhigh` | X upper boundary of iPhi sector, in mm, at `ReadoutSectorEta::fPos_Y`
`iStripNum_Min` | lower bound of strip number for this iPhi sector, e.g. 0, 128, 256
`iStripNum_Max` | upper bound of strip number for this iPhi sector, e.g. 127, 255, 383
`map_slices` | Stores `Uniformity::AnalysisSetupUniformity::iUniformityGranularity` number of `Uniformity::SectorSlice` objects
`map_clusters` | Stores clusters located in this iPhi sector.  Here the key value is the event number the Cluster is associated with
`map_hits` | As `map_clusters` but for Hits

The copy constructor and overloaded assignment operator perform a deep copy of the `std::shared_ptr` objects above.

#### 4.b.viii. DetectorMPGD
Coming "soon"

## 4.c. Utilities
For utility functions defined in the `Timing` namespace see `include/TimingUtilityFunctions.h` and it's implementation in `src/TimingUtilityFunctions.cpp`. For utility functions defined in the `Uniformity` namespace see `include/UniformityUtilityFunctions.h` and it's implementation in `src/UniformityUtilityFunctions.cpp`.

### 4.c.i. Timing
Coming "soon"

### 4.c.ii. Uniformity
Coming "soon"

### 4.c.iii. Plotter
Coming "soon"

## 4.d Types
For types defined in the `Timing` namespace see `include/TimingUtilityTypes.h` and it's implementation in `src/TimingUtilityTypes.cpp`. For utility functions defined in the `Uniformity` namespace see `include/UniformityUtilityTypes.h` and it's implementation in `src/UniformityUtilityTypes.cpp`.

### 4.d.i. Timing
More Coming "soon"

The `Timing::HistoSetup` struct is used for storing user input defined in the *Analysis Config* file. Objects of this struct store values to be set to data members of `TH1` and `TF1` objects. An instance of the `Timing::HistoSetup` struct is used by private methods of the `Uniformity::AnalyzeResponseUniformity` class for producing & manipulating `TH1` & `TF1` objects.

Data members of `Timing::HistoSetup` struct relevant to the `ROOT::TH1` class are shown in the table below:

Data Member | Description
----------- | -----------
`HistoSetup::fHisto_xLower` | lower x range of a `TH1` object
`HistoSetup::fHisto_xUpper` | upper x range of a `TH1` object
`HistoSetup::iHisto_nBins` | number of bins a `TH1` object will have
`HistoSetup::strHisto_Name` | `TName` of a `TH1` object
`HistoSetup::strHisto_Title_X` | X-axis title of a `TH1` object
`HistoSetup::strHisto_Title_Y` | Y-axis title of a `TH1` object

Data members of `Timing::HistoSetup` struct relevant to the `ROOT::TF1` class are shwon in the table below:

Data Member | Description
----------- | -----------
`HistoSetup::strFit_Formula` | `TFormula` of a `TF1` object
`HistoSetup::strFit_Name` | `TName` of a `TF1` object
`HistoSetup::strFit_Option` | Option to be used when fitting some `TObject` with a `TF1`
`HistoSetup::vec_strFit_ParamMeaning` | Vector storing meaning of each fit parameter
`HistoSetup::vec_strFit_ParamIGuess` | Vector storing initial guess of each parameter (`stof` conversion occurs)
`HistoSetup::vec_strFit_ParamLimit_Min` | Vector storing lower bound of each parameter (`stof` conversion occurs)
`HistoSetup::vec_strFit_ParamLimit_Max` | Vector storing upper bound of each parameter (`stof` conversion occurs)
`HistoSetup::vec_strFit_Range` | Vector storing range of `TF1` object

In the case of the vector members above, `stof` conversion occurs via the utility function `Timing::stofSafe`. 

The following members of `Timing::HistoSetup` are not used presently (legacy from `CMS_GEM_TB_Timing`):

- `HistoSetup::bFit`
- `HistoSetup::bFit_AutoRanging`
- `HistoSetup::bIsTrig`
- `HistoSetup::iTDC_Chan`

### 4.d.ii. Uniformity
The list of structs defined in namespace Uniformity is as follows:

- `AnalysisSetupUniformity`
- `Cluster`
- `HistoPhysObj`
- `Hit`
- `RunSetup`
- `SectorEta`
- `SectorPhi`
- `SectorSlice`
- `SelParam`
- `SummaryStatistics`

Each of these items are described in detail below.

Several of the above structs have both copy constructors and overloaded assignment operators which perform a [deep copy](http://stackoverflow.com/questions/184710/what-is-the-difference-between-a-deep-copy-and-a-shallow-copy) of all the objects stored in the struct.  

#### 4.d.ii.I AnalysisSetupUniformity
The `AnalysisSetupUniformity` struct stores user input defined in the *Analysis Config* file.  This struct has one instance of a `HistoSetup` struct for each cluster physical obserable (e.g. ADC, position, etc...).  Additionally this struct has one instance of a `SelParam` struct; which stores user input defined in the *Analysis Config* file for cluster selection.

Data members of `Uniformity::AnalysisSetupUniformity` are:

Data Member | Description
----------- | -----------
`AnalysisSetupUniformity::iEvt_First` | First event of `TTree` to process.
`AnalysisSetupUniformity::iEvt_Total` | Total events of a `TTree` to process.
`AnalysisSetupUniformity::iUniformityGranularity` | The level of granularity the analysis will be carried out on  all iPhi sectors of a `DetectorMPGD` object (e.g. a value of 128 means at the strip level, a value of 2 means two groups of 64 strips).
`AnalysisSetupUniformity::histoSetup_clustADC` | `HistoSetup` obejct for specifying cluster ADC `TH1` and `TF1` objects
`AnalysisSetupUniformity::histoSetup_clustMulti` | `HistoSetup` obejct for specifying cluster multiplicity `TH1` objects
`AnalysisSetupUniformity::histoSetup_clustPos` | `HistoSetup` obejct for specifying cluster position `TH1` objects
`AnalysisSetupUniformity::histoSetup_clustSize` | `HistoSetup` obejct for specifying cluster size `TH1` objects
`AnalysisSetupUniformity::histoSetup_clustTime` | `HistoSetup` obejct for specifying cluster time bin `TH1` objects
`AnalysisSetupUniformity::histoSetup_hitADC` | `HistoSetup` object for specifying hit ADC `TH1` objects
`AnalysisSetupUniformity::histoSetup_hitMulti` | `HistoSetup` obejct for specifying hit multiplicity `TH1` objects
`AnalysisSetupUniformity::histoSetup_hitPos` | `HistoSetup` object for specifying hit position `TH1` objects (in strip No.)
`AnalysisSetupUniformity::histoSetup_hitTime` | `HistoSetup` object for specifying hit time bin `TH1` objects
`AnalysisSetupUniformity::selClust` | `SelParam` object specifying cluster selection cuts
`AnalysisSetupUniformity::selHit` | `SelParam` object specifying hit selection cuts

#### 4.d.ii.II Cluster
The `Uniformity::Cluster` struct stores information relating to one reconstructed cluster stored in the `TCluster` tree.  Data members of `Uniformity::Cluster` are:

Data Member | Description
----------- | -----------
`Cluster::iPos_Y` | Distance in mm from wide base of trapezoid to cluster (e.g. vertical midpoint of iEta sector)
`Cluster::fPos_X` | Horizontal position within iEta sector (used to assign to correct iPhi sector)
`Cluster::fADC` | ADC value of cluster
`Cluster::iSize` | Number of strips in cluster
`Cluster::iTimeBin` | Time bin, e.g. latency value, of the cluster
`Cluster::map_hits` | `std::map` of `Uniformity::Hit` objects used to reconstruct this cluster.  Note presently just a placeholder.  Empty for all clusters presently.

#### 4.d.ii.III Event
The `Uniformity::Event` struct stores information relating to the physics objects contained in an event.  Attributes of `Uniformity::Event` are:

Data Member | Description
----------- | -----------
`Event::iNum_Evt` | Event number
`Event::iNum_Run` | Run number
`Event::vec_clusters` | Vector of Uniformity::Cluster objects
`Event::vec_hits` | Vector of Uniformity::Hit objects
`Event::clear()`  | sets all ints to -1, and clears all stl containers

There is also one copy constructor

#### 4.d.ii.IV HistosPhysObj
The `Uniformity::HistosPhysObj` struct is used as a container for `ROOT` histograms (i.e. `TH1`, `TH2`, etc...). These histograms are tracked at varying levels of the `DetectorMPGD` geometry (e.g. per `SectorEta`, per `SectorPhi`, etc...).  Each data member of `Uniformity::HistosPhysObj` is a `std::shared_ptr` of a `ROOT` object, they are given specifically as:

Data Member | Description
----------- | -----------
`HistosPhysObj::hADC` | ADC Spectrum for some physics object (e.g. clusters, hits, etc...)
`HistosPhysObj::hMulti` | Multiplicity "                                                    "
`HistosPhysObj::hPos` | Position     "                                                    "
`HistosPhysObj::hSize` | Size         "                                                    "
`HistosPhysObj::hTime` | Time bin (e.g. latency) "                                         "
`HistosPhysObj::hADC_v_Pos` | ADC vs Position "                                                 "
`HistosPhysObj::hADC_v_Size` | ADC vs Size "                                                     "
`HistosPhysObj::hADC_v_Time` | ADC vs Latency "                                                  "
`HistosPhysObj::hADCMax_v_ADCInt` | Max ADC (from all latency bins) of an object vs Integral of object's ADC (sum of all latency bins)
`HistosPhysObj::map_hADC_v_EvtNum_by_Run` | `std::map` of `TH2F` objects, each `TH2F` shows ADC vs evt no. for a given run (i.e. input file)
`HistosPhysObj::map_hTime_v_EvtNum_by_Run` | As `HistosPhysObj::map_hADC_v_EvtNum_by_Run` but for Time

For clusters hPos is the position along the detector trapezoid in mm with the detector axis being 0 mm in the iPhi=2 sector, negative (positive) position values occur in iPhi = 1 (3).  For hits the position is the strip number along the detector from 1 to 384 increasing with increasing iPhi.

There is also one copy constructor and one overloaded assignment operator.  These items perform a deep copy of the `std::shared_ptr` objects above.

#### 4.d.ii.V Hit
The `Uniformity::Hit` struct stores information relating to one reconstructed hit stored in the `THit` tree.  Data members of `Uniformity::Hit` are:

Data Member | Description
----------- | -----------
`Hit::iPos_Y` | Distance in mm from wide base of trapezoid to hit (e.g. vertical midpoint of iEta sector)
`Hit::iStripNum` | Strip number of the hit, this ranges from 0 to 383?
`Hit::iTimeBin` | Time bin, e.g. latency value, of the hit
`Hit::sADCIntegral` | Integral of ADC values from all time bins
`Hit::vec_sADC` | `std::vector` of ADC values, each element of the vector represents ADC value at that time bin; e.g. `vec_sADC[Hit::iTimeBin]` gives the ADV value at the defined time bin

#### 4.d.ii.VI RunSetup
The `Uniformity::RunSetup` struct stores user input defined in the `[RUN_INFO]` header of the *Run Config* File.  It does not store the list of input files.  This struct is responsible for setting the analysis configuration and is used by `InterfaceAnalysis` class for identifying what stages of the analysis should be performed. Data members of `Uniformity::RunSetup` are:

Data Member | Description
----------- | -----------
`RunSetup::strRunMode` | string, tracks whether executable is doing analysis or comparison
`RunSetup::bAnaStep_Clusters` | True: perform the cluster analysis; false: do not.
`RunSetup::bAnaStep_Fitting` | True: fit stored distributions; false: do not.  Note `bAnaStep_Clusters` (`bAnaStep_Hits`) must also be true for cluster (hit) distributions to be fitted.
`RunSetup::bAnaStep_Hits` | True: perform the hit analysis; false: do not.
`RunSetup::bAnaStep_Reco` | True: reconstruct clusters from hits; false: take clusters from input `TClusters` tree.
`RunSetup::bAnaStep_Visualize` | True: make summary `TCanvas` objects after analyzing all input files; false: do not.
`RunSetup::strIdent` | string, unique identifier in input filenames
`RunSetup::strObsName` | string, name of observable to be compared across input files in comparsion mode
`RunSetup::iEta` | int, iEta index to be used in comparsion mode
`RunSetup::iPhi` | int, iPhi index to be used in comparsion mode
`RunSetup::iSlice` | int, iSlice index to be used in comparsion mode
`RunSetup::strDetName` | Stores a string acting as the unique detector serial number.  Resolves ambiguity in TObject `TName` data members when opening multiple output TFiles.
`RunSetup::bInputFromFrmwrk` | False (true): input files do (not) contain the `TCluster` or `THit` trees
`RunSetup::bLoadSuccess` | True (false): the input parameters were (not) loaded successfully from the *Run Config* File. Defaults to false.  If after attempting to load these parameters from the *Run Config* File this is still false the analysis routine exits.
`RunSetup::bMultiOutput` | True: one output file is made which represents the sum of all input files; false: one output file is made for each input file.  Note when `bInputFromFrmwrk` is true `bMultiOutput` must also be true.
`RunSetup::strFile_Config_Ana` | PFN of input *analysis config* file
`RunSetup::strFile_Config_Map` | PFN of input *mapping config* file
`RunSetup::strFile_Output_Name` | PFN of output `TFile` to be created when `bMultiOutput` is false
`RunSetup::strFile_Output_Option` | Option for `TFile`, e.g. `CREATE`, `RECRATE`, `UPDATE`, etc...
`RunSetup::bDrawNormalized` | True: in comaprison mode `TH1F` objects are drawn to have area=1; false: no change
`RunSetup::bVisPlots_PhiLines` | True: draw lines denoting iPhi sectors in plots spanning iEta sectors; false: do not.
`RunSetup::bVisPlots_AutoSaving` | True: automatically save `TCanvas` objects stored in the Summary folder of the output `TFile` as `*.png` and `*.pdf` files; false: do not.
`RunSetup::strDrawOption` | string, draw option to be used in comparison mode

#### 4.d.ii.VII SectorSlice
The data members of the `Uniformity::SectorSlice` struct are:

Data Member | Description
----------- | -----------
`SectorSlice::bFitAccepted` | True: fit of `SectorSlice::hSlice_ClustADC` passed quality checks
`SectorSlice::fPos_Center` | Location of the center of the slice, in mm, within the `SectorPhi` (iPhi value)
`SectorSlice::fWidth` | Width of the slice in mm
`SectorSlice::iMinuitStatus` | Minuit status code after attempting to fit `SectorSlice::hSlice_ClustADC`
`SectorSlice::fitSlice_ClustADC` | `std::shared_ptr` of a `TF1`; used to fit `SectorSlice::hSlice_ClustADC`
`SectorSlice::hSlice_ClustADC` | As `SectorEta::hEta_ClustADC` but only for this `SectorSlice`

There is also one copy constructor and one overloaded assignment operator.  These items perform a deep copy of the `std::shared_ptr` objects above.

#### 4.d.ii.VIII SelParam
Data members of `Uniformity::SelParam` are:

Data Member | Description
----------- | -----------
`SelParam::iCut_ADCNoise` | Hit or Cluster rejected if ADC *less than* value
`SelParam::iCut_ADCSat` | Hit rejected if ADC *greater than* value
`SelParam::iCut_MultiMin` | Event rejected if cluster multiplicity *less than or equal to* value
`SelParam::iCut_MultiMax` | Event rejected if cluster multiplicity *greater than or equal to* value
`SelParam::iCut_SizeMin` | Cluster rejected if size *less than* value
`SelParam::iCut_SizeMax` | Cluster rejected if size *greater than* value
`SelParam::iCut_TimeMin` | Hit or Cluster rejected if time bin *less than* value
`SelParam::iCut_TimeMax` | Hit or Cluster rejected if time bin *greater than* value

#### 4.d.ii.IX SummaryStatistics
The `Uniformity::SummaryStatistics` is a container for storing statistical parameters of a dataset (e.g. fit results from all strips).  Data members of `Uniformity::SummaryStatistics` are:

Data Member | Description
----------- | -----------
`SummaryStatistics::fIQR | Interquantile range of dataset; `IQR = Q3 - Q1`
`SummaryStatistics::fMax | Max value of dataset
`SummaryStatistics::fMean | Mean value of dataset
`SummaryStatistics::fMin | Min value of dataset
`SummaryStatistics::fQ1 | First quantile (Q1) of dataset
`SummaryStatistics::fQ2 | Second quantile (Q2) of dataset
`SummaryStatistics::fQ3 | Third quantile (Q3) of dataset
`SummaryStatistics::fStdDev | Standard deviation of dataset
`SummaryStatistics::mset_fOutliers | Container storing outliers of a dataset; outlier definition is defined per Analyzer
`SummaryStatistics::hDist | `TH1F` object of which stores the distribution of the dataset
`SummaryStatistics::fitDist | `TF1` object which attempts a Gaussian fit to `SummaryStatistics::hDist`
`SummaryStatistics::clear() | resets all floats to -1, resets TObjects, and clears all stl containers

There is also one copy constructor and one overloaded assignment operator.  These items perform a deep copy of the `std::shared_ptr` objects above.

### 4.d.iii. Plotter
Coming "soon"

## 4.e. Configuration Files
Three configuration files are require to run the frameworkMain exectuable:

1. the *mapping config* file,  
2. the *analysis config* file, and
3. the *reco config* file.

Examples of each files have been included in the `$GEM_BASE/config` directory

### 4.e.i. Mapping Config File
The *mapping config* file is the file used when analyzing data collected with the SRS system. An example of the mapping config file is shown as:

```
    # CMSGEM
    #################################################################################################
    #   ReadoutType  DetType    DetName    Sector     SectPos   SectSize   nbConnect  orient
    #################################################################################################
    DET,   CMSGEM,    CMSGEM,    CMS,    CMSSECTOR1,    507.475,         411.402,       3,       1
    ...     ...         ...      ...        ...         ...          ...        ...     ...
    ...     ...         ...      ...        ...         ...          ...        ...     ...
    DET,   CMSGEM,    CMSGEM,    CMS,    CMSSECTOR8,    -507.475,        231.186,       3,       1

    ############################################################################################
    #APV     fecId    adcCh     detPlane  apvOrient  apvIndex    apvHdr   apvCustMap
    ############################################################################################ 
    APV,        2,        0,       CMSSECTOR8,     0,      2,        1300,  1
    APV,        2,        1,       CMSSECTOR8,     0,       0,        1300, 1
    
    APV,        2,        2,       CMSSECTOR8,     0,       1,        1300, 0
    APV,        2,        3,       CMSSECTOR7,     0,       1,        1300, 0
    ...     ...         ...      ...        ...         ...          ...        ...     ...
    ...     ...         ...      ...        ...         ...          ...        ...     ...
```

For each "DET" line defined in this file an entry in the `DetectorMPGD::map_sectorsEta` map will be stored.  In this way the mapping config file specifies the geometry of the `DetectorMPGD` object.  For each "APV" line data from that APV25 Hybrid will be linked to the corresponding position in the listed (iEta,iPhi) sector.

Right now the framework only supports detectors with 1D readouts.  

### 4.e.ii. Analysis Config File
The analysis config file expects a certain "nested-header" style. The format should look something like:

```
[BEGIN_ANALYSIS_INFO]
    ...
    ...
    [BEGIN_UNIFORMITY_INFO]
        ...
        ...
        [BEGIN_ADC_FIT_INFO]
            ...
            ...
        [END_ADC_FIT_INFO]
        [BEGIN_HISTO_INFO]
            ...
            ...
        [END_HISTO_INFO]
        ...
        ...
        ...
        ...
        [BEGIN_HISTO_INFO]
            ...
            ...
        [END_HISTO_INFO]
    [END_UNIFORMITY_INFO]
[END_ANALYSIS_INFO]
```

For each of the higher level header sections (i.e. `[BEGIN_ANALYSIS_INFO]` and `[BEGIN_UNIFORMITY_INFO]`) the header parameters should *always* be placed before the declaration of lower headers.  Placing top level header parameters after the declaration of lower level headers could lead to undefined behavior or at worst crashes.

Parameters are expected to be entered in the following format:

```    
field_name = 'value';
```
The `field_name` should be on the left followed by an equals sign "=" then the value should be enclosed in single quote marks "'".  A semicolon ";" ends the statement.  Tabs "\t" and spaces outside of the single quotes will be ignored, but will be preserved inside the single quotes.  Text after the ";" will also be ignored.  The template at the end of this subsection showns an example.

The `Uniformity::ParameterLoaderAnalysis` class understands the `#` character to indicate a comment; so it is possible to comment out lines in the *Analysis Config* file you create for ease of use.

#### 4.e.ii.I HEADER PARAMETERS - ANALYSIS_INFO
None for now, placeholder

#### 4.e.ii.II HEADER PARAMETERS - TIMING_INFO
None for now, placeholder

#### 4.e.ii.III HEADER PARAMETERS - UNIFORMITY_INFO
The table below describes the allowed input fields and their data types.

Field Name | Type | Description
---------- | ---- | -----------
`Cut_ClusterADC_Min` | int | clusters with ADC values less than this value are rejected.
`Cut_ClusterMulti_Min` | int | events with clusters multiplicity less than or equal to this value are rejected.
`Cut_ClusterMulti_Max` | int | events with clusters multiplicity greater than or equal to this value are rejected.
`Cut_ClusterSize_Min` | int | clusters with sizes less than this value are rejected.
`Cut_ClusterSize_Max` | int | clusters with sizes greater than this value are rejected.
`Cut_ClusterTime_Min` | int | clusters with time bins less than this value are rejected.
`Cut_ClusterTime_Max` | int | clusters with time bins greater than this value are rejected.
`Cut_HitAdc_Min` | int | hits with ADC value less than this value are rejected.
`Cut_HitAdc_Max` | int | hits with ADV value greater than this value are rejected.
`Cut_HitMulti_Min` | int | events with hit multiplicity less than or equal to this value are rejected.
`Cut_HitMulti_Max` | int | events with hit multiplicity greater than or equal to this value are rejected.
`Cut_HitTime_Min` | int | hits with time bins less than this value are rejected.
`Cut_HitTime_Max` | int | hits with time bins greater than this value are rejected.
`Event_First` | int | the first event in each tree (`TCluster` and/or `THit`) to start from when running the analysis.
`Event_Total` | int | total number of events to process after `Event_First` in each tree. A value of `-1` sets indicates all events from the first event will be processed.
`Uniformity_Granularity` | int | numer of slices, or partitions, to split one iPhi sector into for the response uniformity measurement.

#### 4.e.ii.IV HEADER PARAMETERS - ADC_FIT_INFO
A set of keywords = {`AMPLITUDE`,`FWHM`,`HWHM`,`MEAN`,`PEAK`,`SIGMA`} is presently supported which allows the user to configure complex expressions for the initial guess of fit parameters, their limits, and the fit range.  In the future additional keywords may be added as requested. The table below describes the supported supported and how they define the initial guess for a given fit:

Keyword | Description
------- | -----------
`AMPLITUDE` | The fit parameter is set to the value of the `TH1` bin with the largest content (e.g. `TH1::GetBinContent(TH1::GetMaximumBin() )` ).
`FWHM` | The fit parameter is set to the RMS of the distribution stored in the `TH1` object (e.g. `TH1::GetRMS()` ).
`HWHM` | The fit parameter is set to half the RMS of the distribution stored in the `TH1` object (e.g. `TH1::GetRMS()` ).
`MEAN` | The fit parameter is set to the mean of the distribution stored in the `TH1` object (e.g. `TH1::GetMean()` ).
`PEAK` | An instance of the `ROOT::TSpectrum` class is created and searches the `TH1` object that will be fitted for all peaks.  The `TSpectrum` parameters are hard coded and "should" ensure only one peak is found (open issue).  The x-position of the first peak found in the `TH1` distribution is set to the fit parameter (e.g. `TSpectrum::Search()` and `TSpectrum::GetPositionX()` are used).
`SIGMA` | As `FWHM`

The framework will calculate the energy resolution of a given slice of the detector.  The energy resolution is always taken as:

```
E_Res = PEAK_FWHM / PEAK_Pos
```

In your fit formula you must label one parameter in the `Fit_Param_Map` (see table below) from the *peak width* set {`FWHM`,`HWHM`,`SIGMA`}.  Failure to do so will cause the energy resolution to be zero for all slices (i.e. all points in `gEta_ClustADC_Fit_PkRes` will be 0 for all `SectorEta` objects defined).  The table below shows how the `PEAK_FWHM` is determined for each of the entires in the peak width set:

Keyword | Expression | Description
------- | ---------- | -----------
`FWHM` | `E_Res = FWHM / PEAK_Pos` | Here `FWHM` is the value of the fit parameter, post fit, given the meaning `FWHM` in the `Fit_Param_Map` field.
`HWHM` | `E_Res = (2.*HWHM) / PEAK_Pos` | Here `HWHM` is the value of the fit parameter, post fit, given the meaning `HWHM` in the `Fit_Param_Map` field.
`SIGMA` | `E_Res = (2. * sqrt( 2. * ln(2.) ) * SIGMA ) / PEAK_Pos` | Here `SIGMA` is the value of the fit parameter, post fit, given the meaning `SIGMA` in the `Fit_Param_Map` field. 

The table below describes the allowed input fields and their data types:

Field Name | Type | Description
---------- | ---- | -----------
`Fit_Formula` | string | `TFormula` to be given to a `ROOT::TF1` object.  Note the syntax and full complexity of inputs expected/available in `ROOT` works! e.g. `[0]*x^2+[1]` or `gaus(0)+pol2(3)" or "[1]*TMath::Erf(x) + [2]` are all supported.
`Fit_Formula_Sig` | string | As `Fit_Formula` but this is understood to be the portion of the formula which describes the signal peak.  Note the parameters here must be indexed from 0 and follow the same order as they due in `Fit_Formula`. E.g. if your `Fit_Formula` is `pol2(0) + [3]*TMath::CauchyDist(x,[4],[5])` with the `CauchyDist` describing your signal peak then `Fit_Formula_Sig` would be written as `[0]*TMath::CauchyDist(x,[1],[2])`.  Here the index of the parameters has been restarted at [0] but the order is preserved. This, in conjunction with `Fit_Formula_Sig_Param_Idx_Range`, will ensure the parameter determined for [3] in `Fit_Formula` is assigned to [0] in `Fit_Formula_Sig`.
`Fit_Formula_BKG` | string | As `Fit_Formula_Sig` but for the background. 
`Fit_Formula_Sig_Param_Idx_Range` | Two comma separated int's | Gives the range of the parameters that correspond to the signal peak in `Fit_Formula`. E.g. if your `Fit_Formula` is `pol2(0) + [3]*TMath::CauchyDist(x,[4],[5])` with the `CauchyDist` describing your signal peak then `Fit_Formula_Sig_Param_Idx_Range` is **"3,5"** which, in conjunction with `Fit_Formula_Sig`, will ensure the parameter determined for `[3]` in `Fit_Formula` is assigned to `[0]` in `Fit_Formula_Sig`.
`Fit_Formula_Sig_Param_Idx_Range` | Two comma separated int's | As `Fit_Formula_Sig_Param_Idx_Range` but for the background.
`Fit_Name` | string | Starting point of `TName` of the `TF1` object; note the `(ieta,iphi,islice)` coordinate of the fit will be used to augment the `TName` to ensure a unique `TName` for each `TF1` object created.
`Fit_Option` | string | The fit option to be used for fitting the ADC spectrums made from each slice.
`Fit_Param_IGuess` | Comma separated list of strings | Initial values for the parameters defined in the `Fit_Formula` field.  Note the order in which the parameters are given in the `Fit_Formula` field should match the order listed here. Explicitly for the `Fit_Formula = '[0]*x^2+[1]*x+[2]'` the initial guess of `4,5,6` would mean `[0] = 4`, `[1] = 5`, and `[2] = 6`.  Additionally, both numeric values and expressions formed from the above supported keywords can be used.  E.g. for a `Fit_Formula = 'gaus(0)'` the initial guess of `12.3, MEAN, 2.*SIGMA+23.5` can be assigned.
`Fit_Param_Limit_Max` | Comma separated list of strings | As `Fit_Param_IGuess` but for the upper limit on fit parameters.
`Fit_Param_Limit_Min` | Comma separated list of strings | As `Fit_Param_IGuess` but for the lower limit on fit parameters.
`Fit_Param_Map` | Comma separated list of strings | Words to help the user track the meaning of the fit parameter. The order follows the ordering schema described in `Fit_Param_IGuess`. NOTE: at least one parameter *must* be given the value `PEAK` and one parameter *must* be given a value from the set {`FWHM`,`HWHM`,`SIGMA`}. The vertical error-bar on these two parameters comes from the error on the corresponding fit parameter determined in `ROOT` (e.g. `TF1::GetParError()` )
`Fit_Range` | Comma separated list of strings | As `Fit_Param_IGuess` but for the fit range.  NOTE: must supply exactly two parameters.  If more than two parameters are supplied only those that evaluate to the maximum and minimum are used.
        
#### 4.e.ii.V HEADER PARAMETERS - HISTO_INFO
The user at runtime is able to specify the data members of the `TH1` objects that will be created (e.g. number of bins, `TName`, etc...).  However, these histograms are hard coded and the user cannot choose at runtime to make new distributions.  If a distribution you would like to see does not already exist please navigate to the [issue tab](https://github.com/bdorney/CMS_GEM_Analysis_Framework/issues) and submit a new feature request describing your requested distribution. The table below describes the allowed input fields and their data types.

Field Name | Type | Description
---------- | ---- | -----------
`Histo_Name` | string | Type of histogram you are specifying information for.  NOTE: this entry should appear as the *first* line below the section header (`[BEGIN_HISTO_INFO]`) and only entries from the following set are allowed: {`clustADC`, `clustMulti`, `clustPos`, `clustSize`, `clustTime`, `hitADC`, `hitMulti`, `hitPos`, `hitTime`}.
`Histo_XTitle` | string | Title of the x-axis, full `TLatex` style entires are supported. Explicitly `cluster position #left(#mum#right)` will result in a well formatted x-axis with parenthesis that are sized accordingly and proper Greek lettering.
`Histo_YTitle` | string | As `Histo_XTitle` but for the y-axis.
`Histo_BinRange` | Two comma separated floats | Specifies the lowest and highest values of the x-axis.  In the case of `Histo_Name = {clustPos, hitPos}` this field is automatically set based on input from *mapping config* file.
`Histo_NumBins` | int | number of bins in the histogram.  In the case of `Histo_Name = {clustPos, hitPos}` then this field is automatically set based on input from the *mapping config* file.

#### 4.e.ii.VI Example Config File
An example config file is shown below:

```
[BEGIN_ANALYSIS_INFO]
    [BEGIN_UNIFORMITY_INFO]
        #Selection Cuts
        ####################################
        Cut_ClusterADC_Min = '300';
        Cut_ClusterMulti_Min = '0';
        Cut_ClusterMulti_Max = '20';
        Cut_ClusterSize_Min = '2';
        Cut_ClusterSize_Max = '10';
        Cut_ClusterTime_Min = '6';
        Cut_ClusterTime_Max = '27';
        #Selection Cuts - Hit
        ####################################
        Cut_HitAdc_Min = '60';
        Cut_HitAdc_Max = '3000';
        Cut_HitMulti_Min = '1';
        Cut_HitTime_Min = '2';
        Cut_HitTime_Max = '29';
        #Event Range
        ####################################
        Event_First = '0';
        Event_Total = '-1';
        #Requested Granularity
        ####################################
        Uniformity_Granularity = '64';
        #Requested Tolerance on Uniformity
        ####################################
        [BEGIN_ADC_FIT_INFO]
            Fit_Formula = '[0]*TMath::CauchyDist(x, [1], [2])+pol5(3)';
            Fit_Formula_Sig = '[0]*TMath::CauchyDist(x, [1], [2])';
            Fit_Formula_Sig_Param_Idx_Range = '0,2';
            Fit_Formula_Bkg = 'pol5';
            Fit_Formula_Bkg_Param_Idx_Range = '3,8';
            Fit_Param_Map = 'AMPLITUDE, PEAK, HWHM';
            Fit_Param_IGuess = '1000000,PEAK,PEAK*0.3';
            Fit_Param_Limit_Min = '10, PEAK-0.2*PEAK, 0.1*PEAK';
            Fit_Param_Limit_Max = '10000000, PEAK+0.2*PEAK, 0.70*PEAK';
            Fit_Range = 'PEAK-0.57*PEAK, 3*PEAK';
        [END_ADC_FIT_INFO]
        [BEGIN_HISTO_INFO]
            Histo_Name = 'clustADC';
            Histo_XTitle = 'Cluster ADC';
            Histo_YTitle = 'N';
            Histo_BinRange = '0,10000';
            #Histo_NumBins = '150';
            Histo_NumBins = '100';
        [END_HISTO_INFO]
        [BEGIN_HISTO_INFO]
            Histo_Name = 'clustMulti';'
            Histo_XTitle = 'Cluster Multiplicity';
            Histo_YTitle = 'N';
            Histo_BinRange = '0,20';
            Histo_NumBins = '20';
        [END_HISTO_INFO]
        [BEGIN_HISTO_INFO]
            Histo_Name = 'clustPos';
            Histo_XTitle = 'Cluster Position #left(mm#right)';
            Histo_YTitle = 'N';
            #Here Histo_BinRange is set automatically based on input amoreSRS mapping file
            #Here Histo_NumBins is set automatically based off Bin_Range
        [END_HISTO_INFO]
        [BEGIN_HISTO_INFO]
            Histo_Name = 'clustSize';
            Histo_XTitle = 'Size #left(N_{strips}#right)';
            Histo_YTitle = 'N';
            Histo_BinRange = '0,20';
            Histo_NumBins = '20';
        [END_HISTO_INFO]
        [BEGIN_HISTO_INFO]
            Histo_Name = 'clustTime';
            Histo_XTitle = 'Time Bin';
            Histo_YTitle = 'N';
            Histo_BinRange = '0,30';
            Histo_NumBins = '30';
        [END_HISTO_INFO]
        [BEGIN_HISTO_INFO]
            Histo_Name = 'hitADC';
            Histo_XTitle = 'Hit ADC';
            Histo_YTitle = 'N';
            Histo_BinRange = '0,2000';
            Histo_NumBins = '200';
        [END_HISTO_INFO]
        [BEGIN_HISTO_INFO]
            Histo_Name = 'hitMulti';'
            Histo_XTitle = 'Hit Multiplicity';
            Histo_YTitle = 'N';
            Histo_BinRange = '0,50';
            Histo_NumBins = '50';
        [END_HISTO_INFO]
        [BEGIN_HISTO_INFO]
            Histo_Name = 'hitPos';
            Histo_XTitle = 'Hit Position #left(mm#right)';
            Histo_YTitle = 'N';
            #Here Histo_BinRange is set automatically based on input amore mapping file
            #Here Histo_NumBins is set automatically based off Bin_Range
        [END_HISTO_INFO]
        [BEGIN_HISTO_INFO]
            Histo_Name = 'hitTime';
            Histo_XTitle = 'Time Bin';
            Histo_YTitle = 'N';
            Histo_BinRange = '0,30';
            Histo_NumBins = '30';
        [END_HISTO_INFO]
    [END_UNIFORMITY_INFO]
[END_ANALYSIS_INFO]
```
### 4.e.iii. Run Config File
The run config file expects a certain "header" style. The format should look something like:

```
[BEGIN_RUN_INFO]
    ...
    ...
    ...
[END_RUN_INFO]
[BEGIN_RUN_LIST]
    ...
    ...
    ...
[END_RUN_LIST]
```

Parameters found inside the `[BEGIN_RUN_INFO]` header are expected to be entered in the manner described in Section 4.e.ii.

Contrary to the `[BEGIN_RUN_INFO]` header the `[BEGIN_RUN_LIST]` header is simply a list of PFN of the input files to be analyzed by the call of the executable.  Again tabs `\t` and spaces will be ignored.

The `Uniformity::ParameterLoaderRun` class understands the `#` character to indicate a comment; so it is possible to comment out lines in the *Run Config* file you create for ease of use.  The value of `true` is understood as being from the case-insensitive set `{t, true, 1}` while the value of `false` is understood as being from the case-insensitive set `{f, false, 0}`. Example config files are shown at the end of this subsection.

#### 4.e.iii.I HEADER PARAMETERS - RUN INFO
The table below describes the allowed input fields and their data types.

Field Name | Type | Description
---------- | ---- | -----------
`Config_Analysis` | string | PFN of the input *analysis config* file.
`Config_Mapping` | string | PFN of the input *mapping config* file.
`Config_Reco` | string | PFN of the input *reco config* file (e.g. old "amore.cfg" file).
`Detector_Name` | string | the serial number of the detector (do not include special characters such as '/' but dashes '-' are allowed)
`Input_Identifier` | string | a regular expression found in each input filename, separated by underscores `_`, that is understood to have the run number after the expression. E.g. if the filename contains `_RunX_` for some set of integers X then this field should be set to `Run`. This field must always be set.
`Input_Is_Frmwrk_Output` | boolean | set to false (true) if the input file/files (do not) contain the `TCluster` and/or `THit` trees. Note that if this option is set to true then `Output_Individual` must *also* be set to true.
`Output_File_Name` | string | PFN of the output `TFile`.  If `Output_Individual` is set to *true* and `Input_Is_Frmwrk_Output` is set to *false* then the PFN defined here is not used.  Instead the PFN of the input `TFile` is used but the `dataTree.root` ending of the PFN is removed and replaced with `Ana.root`.  If `Input_Is_Frmwrk_Output` is set to true then the PFN defined here is again not used.  Instead the PFN of the input `TFile` is used but the filename is appended with `NewAna.root`.
`Output_File_Option` | string | Write option for the output TFile from the standard set defined in the `TFile` documentation, e.g. {`CREATE`, `NEW`, `READ`, `RECREATE`, `UPDATE`}
`Output_Individual` | bool | Setting to **true** produces one output file for *each* input file. Setting to **false** produces one output file that represents the entirity of the analysis of all input files.  Note that this should only be set to false if `Input_Is_Frmwrk_Output` is *also* set to false.
`Reco_All` | bool | Set to true if input files are raw data files.
`Ana_Hits` | bool | Setting to true will tell the framework to perform the analysis of the input hits.
`Ana_Clusters` | bool | Setting to true will tell the framework to perform the analysis of the input clusters.
`Ana_Fitting` | bool | Setting to true will tell the framework to fit the obtained distributions.  Note that `Ana_Clusters` must also be true for those distributions to be fitted.
`Visualize_Plots` | bool | Setting to true will tell the framework to prepare several `TCanvas` objects after analyzing all input files (`Output_Individual = false`) or each input file (`Output_Individual = true`).
`Visualize_AutoSaveImages` | bool | Setting to true will tell the framework to automatically create `*.png` and `*.pdf` files of all `TCanvas` objects stored in the *Summary* folder. The name of these files will match the `TName` of the corresponding `TCanvas`. They will be found in the working directory (the directory you execute the framework executable from).  If these files already exist they will be over-written.
`Visualize_DrawPhiLines` | bool | Setting to true will tell the framework to draw lines on the summary `TCanvas` objects that show the iPhi segmentation.

#### 4.e.iii.II  HEADER PARAMETERS - RUN LIST
This header contains a list of PFN of the input files.  It is expected that there is one line for per file.  White space, such as tabs `\t` and spaces ` `, is ignored when reading in these input files. For example:

```
[BEGIN_RUN_LIST]
    ...
    ...
    /filepath/filename3.root
    /filepath/filename4.root
    ...
    ...
[END_RUN_LIST]
```

#### 4.e.iii.III  HEADER PARAMETERS - COMP INFO
The table below describes the allowed input fields and their data types.

Field Name | Type | Description
---------- | ---- | -----------
`Obs_Name` | string | The **ObservableNameX** (See Section 4.f.i) found in the `TName` of the `TH1F` object that you wish to compare across all input files.
`Obs_Eta` | int | the iEta index you wish to comapre `TH1F` objects from.  This must be from [1, iNumEta] or -1.  If set to -1 summary level `TH1F` objects are compared.
`Obs_Phi` | int | As `Obs_Eta` but for phi index.
`Obs_Slice` | int | As `Obs_Eta` but for slice index.
`Input_Identifier` | string | Regular expression found in each input filename, separated by underscores `_`.  From each filename this expression will be drawn on a `TLegend` to identify the `TH1F` object from the corresponding filename.
`Output_File_Name` | string | As Section 4.e.iii.I
`Output_File_Option` | string | As Section 4.e.iii.I
`Visualize_AutoSaveImages` | bool | As Section 4.e.iii.I
`Visualize_DrawNormalized` | bool | Setting to true will tell the framework all distributions plotted should be re-scaled to have an integral = 1.
`Visualize_DrawOption`     | string | Draw option to use for selected distributions.

#### 4.e.iii.IV  Configuration Options
The framework can run with: 

1. an analysis option, which has three configurations;
2. a comparison option, which has one configuration; 
3. a reconstruction option, which has one configuration; and 
4. a combined reconstruction and analysis option which has one configuration. 

The three configurations of the *analysis mode* have the `frameworkMain` executable analyze data taken with *RD51 Scaleable Readout System (RD51 SRS)* and reconstructed the framework. In the *comparison mode* the `frameworkMain` executable is used to compare plots across multiple Framework output files.  In the *reconstruction option* the `frameworkMain` executable takes reconstructs **one** raw data file produced by the *RD51 SRS* system to create an output tree file for later analysis.  This option *will crash* if *more than one* input file `*.raw` file is used.  In the *combined option* the `frameworkMain` executable is used to perform the reconstruction and then analysis of an input *RD51 SRS* raw data file.  Again in this option only a single input file must be given; however here two output root files will be produced, one a tree file, the other the framework output file.

The 1st *analysis configuration* is the **series** mode which will analyze all of the input files defined in the `[BEGIN_RUN_LIST]` header one after another; note these input files must either be all tree files or all analyzed files but *not* a mix of both. The time of execution can vary depending on the number of input files and the number of events/slice granularity present in each of those files. However, care has been taken to maximimize performance while still maintaining modularity/flexibility in the design.  Here the `[BEGIN_RUN_INFO]` header can be configured to execute whatever level of the framework analysis is desired; but this will be applied to _each_ of the input files defined in the `[BEGIN_RUN_LIST]` header.  

The 2nd *analysis configuration* is the computing cluster mode; to avoid confusion w/charge clusters this is referred to as **grid** mode. Here the input *run config* file contains a single input file in the `[BEGIN_RUN_LIST]` header and the `[BEGIN_RUN_INFO]` header is configured such that `Ana_Fitting` and `Visualize_Plots` are set to false.  The user uses a script/scheduler of their choice to launch their jobs to a computing cluster to analyze a set of input files in parallel (scripts to do this with the batch submission system on CERN's lxplus are included in the repository).  Then after all jobs are finished and the outputs retrieved the user can merge the output files together (if running over tree files) using the `hadd` command.  Then this merged file can be reprocessed in series mode with `Input_Is_Frmwrk_Output`, `Ana_Fitting`, and `Visualize_Plots` set to true.

The 3rd *analysis configuration* is the **re-run** mode.  Here a `TFile` that has been previously produced by the Framework is re-analyzed after changing the fit parameters defined in the *analysis configuration* file.  Each run will result in a new `TFile` (independent from the input) that has the updated results.  This allows the user to more rapidly study variations in parameters without
having to waste time performing the base selection (which may not need to change).  Right now in **re-run** mode only cluster level plots are propogated to the new `TFile`.  Additionally none of the `TH2F` objects found in the `RunHistory` folder will be propogated to the new `TFile`.

In the *comparison configuration* one can take a set of TFiles that have been previously produced by the Framework and compare `TH1F` objects from the files against each other. Here the input *Run Config* file should be configured to have a `[BEGIN_COMP_INFO]` header instead of the `[BEGIN_RUN_INFO]` header that is used by the other modes.  The `[BEGIN_RUN_LIST]` header is still used normally; however now the input files listed here must be Framework output files produced in one of the three modes described above.

In the *reconstruction configuration* one can take a raw data file recorded with the *RD51 SRS* system and perform the unpacking, decoding, and event reconstruction to produce an output `TFile` which contains a `TTree` which can be analyzed by the Framework.  Here the input *Run Config* file should be configured to have a `[BEGIN_RUN]` header as in the case of the analysis configurations.  The `[BEGIN_RUN_LIST]` header is still used however there should only be _one_ file listed.

In the *combined configuration* the Framework first executes the workflow performed by the reconstruction and then executes the analysis.  Here the input *Run Config* file should be configured to have a `[BEGIN_RUN]` header as in the case of the analysis configurations. The framework will produce two TFiles one will have the output `TTree` produced by the reconstruction configuration and the other will be the standard framework output file produced by the analysis.  The `[BEGIN_RUN_LIST]` header is still used however again there should only be a single file listed.  The framework will automatically determine the name of the `TTree` file it should run over in the analysis portion.

Example configuration files illustrating these options are provided in the sections below.  Furthermore template config files and helper scripts are provided in the framework to run in each mode.  For details on this functionality see Sections 3.a.i through 3.a.iii.

#### 4.e.iii.V  Example Config File - Mode: Series
Four example files here are presented. The first example illustrates a series run in which the entire analysis is requested on a list of input TFile's containing the `THit` and `TCluster` trees.  Here the `Output_Individual` is set to false to create one output file representing the results of the analysis on all input files. Changing `Output_Individual` to true will produce one output file per input file.  The example is as follows:

```
[BEGIN_RUN_INFO]
    #Config Files
    ####################################
    Config_Analysis = 'config/configAnalysis.cfg';
    Config_Mapping = 'config/Mapping_GE11-VII-L.cfg';
    #Detector
    ####################################
    Detector_Name = 'GE11-VII-L-CERN-0002';
    #Input Config
    ####################################
    Input_Is_Frmwrk_Output = 'false';   #indicates we are running on input created by amoreSRS
    Input_Identifier = 'Run';
    #Output Config
    ####################################
    Output_File_Name = 'GE11-VII-L-CERN-0002_Summary_RandTrig_AgXRay40kV100uA_1500kEvt_Ana.root';
    Output_File_Option = 'RECREATE';
    Output_Individual = 'false';        #indicates we are making one output file that represents results obtained from all inputs
    #Analysis Steps
    ####################################
    Ana_Reco_Clusters = 'false';
    Ana_Hits = 'true';
    Ana_Clusters = 'true';
    Ana_Fitting = 'true';
    #Visualizer Config
    ####################################
    Visualize_Plots = 'true';
    Visualize_AutoSaveImages = 'true';
    Visualize_DrawPhiLines = 'true';
[END_RUN_INFO]
[BEGIN_RUN_LIST]
    /base_dir/sub_dir/sub_dir/GE11-VII-L-CERN-0002_Run100_Physics_RandomTrigger_XRay40kV100uA_580uA_500kEvt_dataTree.root
    /base_dir/sub_dir/sub_dir/GE11-VII-L-CERN-0002_Run101_Physics_RandomTrigger_XRay40kV100uA_580uA_500kEvt_dataTree.root
    /base_dir/sub_dir/sub_dir/GE11-VII-L-CERN-0002_Run102_Physics_RandomTrigger_XRay40kV100uA_580uA_500kEvt_dataTree.root
[END_RUN_LIST]
```

here leading tabs are shown just for convenience and can be kept/or omitted without consequence.

The second example illustrates a series run in which only the final portion of the analysis (fitting and visualizing) is performed on an input `TFile` created by the Framework.  Here `Output_Individual` is set to true, as it must be, and one output file for each input file will be produced.  The example is as follows:

```
[BEGIN_RUN_INFO]
    #Config Files
    ####################################
    Config_Analysis = 'config/configAnalysis.cfg';
    Config_Mapping = 'config/Mapping_GE11-VII-S.cfg';
    #Detector
    ####################################
    Detector_Name = 'GE1/1-VII-S-CERN-0001';
    #Input Config
    ####################################
    Input_Is_Frmwrk_Output = 'true';    #indicates we are running on input created by the CMS_GEM_Analysis_Framework
    #Output Config
    ####################################
    Output_File_Option = 'RECREATE';
    Output_Individual = 'true';         #indicates we are making one output file for each input file
    #Analysis Steps
    ####################################
    Ana_Reco_Clusters = 'false';
    Ana_Hits = 'false';
    Ana_Clusters = 'true';              #this is set to true so we fit the cluster distributions
    Ana_Fitting = 'true';
    #Visualizer Config
    ####################################
    Visualize_Plots = 'true';
    Visualize_DrawPhiLines = 'true';
[END_RUN_INFO]
[BEGIN_RUN_LIST]
    /base_dir/sub_dir/sub_dir/GE11-VII-S-CERN-0002_Summary_RandTrig_AgXRay40kV100uA_1500kEvt_ClustSize2_Ana.root
    /base_dir/sub_dir/sub_dir/GE11-VII-S-CERN-0002_Summary_RandTrig_AgXRay40kV100uA_1500kEvt_ClustSize3_Ana.root
    /base_dir/sub_dir/sub_dir/GE11-VII-S-CERN-0002_Summary_RandTrig_AgXRay40kV100uA_1500kEvt_ClustSize4_Ana.root
[END_RUN_LIST]
```

in this case the framework should create the following list of output files:

```
/base_dir/sub_dir/sub_dir/GE11-VII-S-CERN-0002_Summary_RandTrig_AgXRay40kV100uA_1500kEvt_ClustSize2_NewAna.root
/base_dir/sub_dir/sub_dir/GE11-VII-S-CERN-0002_Summary_RandTrig_AgXRay40kV100uA_1500kEvt_ClustSize3_NewAna.root
/base_dir/sub_dir/sub_dir/GE11-VII-S-CERN-0002_Summary_RandTrig_AgXRay40kV100uA_1500kEvt_ClustSize4_NewAna.root
```

If you're filename ends with `*_Ana.root` the input file will not be overwritten and instead a new file will be produced with the same PFN as the input but ending with `*_NewAna.root` instead. Pay special attention to the fact that these files will not necessarily be found in the directory you are calling the executable from but in the directory the input file is found in.

The third example illustrates a series run in which the reconstruction is requested on a list of raw data files taken with the *RD51 SRS*.  This time the *analysis config* file is not supplied but instead a *reco config* file is given. Here the `Output_Individual` is set to true to create one output file for the input file. Only one input file is given.  The example is as follows:

```
[BEGIN_RUN_INFO]
    #Config Files
    ####################################
    Config_Reco = 'config/configReco.cfg';
    Config_Mapping = 'config/Mapping_GE11-VII-L.cfg'; #Make sure this file matches what is in the configReco.cfg file!
    #Input Config
    ####################################
    Input_Is_Frmwrk_Output = 'false';   #indicates we are running on input created by amoreSRS
    Input_Identifier = 'Run';
    #Output Config
    ####################################
    Output_File_Option = 'RECREATE';
    Output_Individual = 'true';         #Here we are having the output PFN be the input PFN appended with "Ana.root"
    #Reco Steps
    ####################################
    Reco_All = 'true';
    #Analysis Steps
    ####################################
    Ana_Hits = 'false';
    Ana_Clusters = 'false';
    Ana_Fitting = 'false';
    #Visualizer Config
    ####################################
    Visualize_Plots = 'false';
[END_RUN_INFO]
[BEGIN_RUN_LIST]
    #Place only one file here, use grid submission mode for multiple files
    /base_dir/sub_dir/sub_dir/GE11-VII-L-CERN-0004_Run024_Physics_615uA_XRay40kV25uA_500kEvt.raw
[END_RUN_LIST]
```

notice that the three analysis flags `Ana_Hits`, `Ana_Clusters`, and `Ana_Fitting` are set to `false` and the reconstruction flag `Reco_All` is set to `true`.  The visualizer has also been turned off by setting `Visualize_Plots` to `false`. No output file name is given here since the framework will automatically create the following output file:

```
/base_dir/sub_dir/sub_dir/GE11-VII-L-CERN-0004_Run024_Physics_615uA_XRay40kV25uA_500kEvt_dataTree.root
```

The final example illustrates a series run in which the combined option (reconstruction plus analysis) is used.  Again the input file is a raw data file taken with the *RD51 SRS*.  Notice that all three config files are given: 

1. reco config, 
2. analysis config, and 
3. mapping config file. 

Here the `Output_Individual` is set to true to create one output file for the input file. Only one input file is given.  The example is as follows:

```
[BEGIN_RUN_INFO]
    #Config Files
    ####################################
    Config_Reco = 'config/configReco.cfg';
    Config_Analysis = 'config/configAnalysis.cfg';
    Config_Mapping = 'config/Mapping_GE11-VII-L.cfg'; #Make sure this file matches what is in the configReco.cfg file!
    #Input Config
    ####################################
    Input_Is_Frmwrk_Output = 'false';   #indicates we are running on input created by amoreSRS
    Input_Identifier = 'Run';
    #Output Config
    ####################################
    Output_File_Option = 'RECREATE';
    Output_Individual = 'true';         #Here we are having the output PFN be the input PFN appended with "Ana.root"
    #Reco Steps
    ####################################
    Reco_All = 'true';
    #Analysis Steps
    ####################################
    Ana_Hits = 'true';
    Ana_Clusters = 'true';
    Ana_Fitting = 'false';
    #Visualizer Config
    ####################################
    Visualize_Plots = 'false';
[END_RUN_INFO]
[BEGIN_RUN_LIST]
    #Place only one file here, use grid submission mode for multiple files
    /base_dir/sub_dir/sub_dir/GE11-VII-L-CERN-0004_Run024_Physics_615uA_XRay40kV25uA_500kEvt.raw
[END_RUN_LIST]
```

notice that `Reco_All` has been set to `true` along with `Ana_Hits` and `Ana_Clusters`.  The visualizer has also been turned off by setting `Visualize_Plots` to `false`. No output file name is given here since the Framework will automatically create the following output files:

```
/base_dir/sub_dir/sub_dir/GE11-VII-L-CERN-0004_Run024_Physics_615uA_XRay40kV25uA_500kEvt_dataTree.root
/base_dir/sub_dir/sub_dir/GE11-VII-L-CERN-0004_Run024_Physics_615uA_XRay40kV25uA_500kEvt_Ana.root
```

The fitting (`Ana_Fitting`) and the visualizer (`Visualize_Plots`) may be turned on by setting the relevant flags to true.  However in this example the input file contains only 500k events which is not enough for a high granularity analysis which usually requires at least 10 million clusters distributed over the detector.

#### 4.e.iii.VI  Example Config File - Mode: Grid
Grid mode is really designed for running the framewok on multiple input files (either `TTree` files or *RD51 SRS* raw data files) in parallel.  One could also use this option when running on multiple input TFiles created by the framework but the increase in analysis speed would be small in comparison since usually you are only interested in checking a new set of fit parameters on the previously obtained data.

It is strongly recommended that you perform this script using the provided `scripts/runMode_Grid.sh` or `script/srunMode_Grid_Reco.sh` scripts (See Sections 3.a.i and 3.a.ii) located in the script/ directory, included in the repository, to a fast queue such as the 8 natural minute (8nm) or 1 natural hour (1nh) queue. The `8nm` queue has been found to be ideal for running the analysis on an input `TTree` file with `N_Evt <= 250k`.  Reconstructing an *RD51 SRS* raw dat of similiar event size has been shown best suited for the `1nh` queue. The two scripts mentioned will setup everything automatically for you.  An example config file is shown as:

```
[BEGIN_RUN_INFO]
    #Config Files
    ####################################
    Config_Analysis = 'config/configAnalysis.cfg';
    Config_Mapping = 'config/Mapping_GE11-VII-L.cfg';
    #Input Config
    ####################################
    Input_Is_Frmwrk_Output = 'false';   #indicates we are running on input created by amoreSRS
    Input_Identifier = 'Run';
    #Output Config
    ####################################
    Output_File_Option = 'RECREATE';
    Output_Individual = 'true';         #Here we are having the output PFN be the input PFN appended with "Ana.root"
    #Analysis Steps
    ####################################
    Ana_Reco_Clusters = 'false';
    Ana_Hits = 'true';
    Ana_Clusters = 'true';
    Ana_Fitting = 'false';              #Do not perform the fitting; input file is a subset of the total dataset
    #Visualizer Config
    ####################################
    Visualize_Plots = 'false';          #Do not perform the visualization; input file is a subset of the total dataset
[END_RUN_INFO]
[BEGIN_RUN_LIST]
    /base_dir/sub_dir/sub_dir/myFileForThisJob.root
[END_RUN_LIST]
```

#### 4.e.iii.VII  Example Config File - Mode: Re-Run
The re-run mode is designed to allow a user to change the fit parameters defined in their analysis config file and re-run on an input TFile previously produced by the framework.  This saves significant time when tweaking the fit parameters being applied to a given input file since the selection does not have to be repeated.  Obviously this mode should not be applied to input `TTree` TFiles produced containing the `THit` and `TCluster` trees. The example config file is given below:

```
[BEGIN_RUN_INFO]
    #Config Files
    ####################################
    Config_Analysis = 'config/configAnalysis.cfg';
    Config_Mapping = 'config/Mapping_GE11-VII-L.cfg';
    #Detector
    ####################################
    Detector_Name = 'DETECTORNAME';
    #Input Config
    ####################################
    Input_Is_Frmwrk_Output = 'true'     #indicates we are running on input created by the framework
    #Output Config
    ####################################
    Output_File_Option = 'RECREATE';
    Output_Individual = 'true';         #must be set to true since Input_Is_Frmwrk_Output = true
    #Analysis Steps
    ####################################
    Ana_Reco_Clusters = 'false';
    Ana_Hits = 'false';
    Ana_Clusters = 'true';              #this is set to true so we fit the cluster distributions
    Ana_Fitting = 'true';
    #Visualizer Config
    ####################################
    Visualize_Plots = 'true'; #true -> make summary canvas plots; false -> do not make summary canvas plots
    Visualize_AutoSaveImages = 'false';
    Visualize_DrawPhiLines = 'true'; #true -> draw iPhi lines; false -> do not draw iPhi lines
[END_RUN_INFO]
[BEGIN_RUN_LIST]
    /base_dir/sub_dir/sub_dir/myPreviousFrmwrkOutput_Detector1.root
[END_RUN_LIST]
```

in this case the Framework should create the following output file:

```
/base_dir/sub_dir/sub_dir/myPreviousFrmwrkOutput_Detector1_NewAna.root
```

again pay special attention to the fact that these files will not necessarily be found in the directory you are calling the executable from but in the directory the input file is found in. Astute readers will note this is identical to the series mode example 2 with just one input file. This is true; however, I felt the explicit example could prove useful.

#### 4.e.iii.VII  Example Config File - Mode: Comparison
The comparison mode is designed to allow a user to quickly compare TH1F objects stored in different framework output files from different detectors, different acquisition conditions, or produced with different analysis parameters.  This can save signficant time for an analyst interested in performing these comparisons.  The example config file is given below:

```
[BEGIN_COMP_INFO]
    #Observable
    ####################################
    Obs_Name = 'clustADC';
    Obs_Eta = '4';
    Obs_Phi = '2';
    Obs_Slice = '-1';
    #Input Config
    ####################################
    Input_Identifier = 'ClustTime';
    #Output Config
    ####################################
    Output_File_Name = 'GE11-VII-L-CERN-0002_ClustTime_Comparison.root';
    Output_File_Option = 'UPDATE';
    #Visualizer Config
    ####################################
    Visualize_AutoSaveImages = 'true';
    Visualize_DrawNormalized = 'false';
    Visualize_DrawOption = "E1"
    Visualize_DrawPhiLines = 'false';
[END_COMP_INFO]
[BEGIN_RUN_LIST]
    /base_dir/sub_dir/sub_dir/GE11-VII-L-CERN-0002_Summary_Physics_RandTrig_AgXRay40kV100uA_580uA_15004kEvt_ClustTime1to30_ClustSize1to20_Ana.root
    /base_dir/sub_dir/sub_dir/GE11-VII-L-CERN-0002_Summary_Physics_RandTrig_AgXRay40kV100uA_580uA_15004kEvt_ClustTime6to27_ClustSize1to20_Ana.root
[END_RUN_LIST]
```

### 4.e.iv. Plot Config File
The plot config file expects a certain "nested-header" style.  The format should look something like:

```
    [BEGIN_CANVAS]
        ...
        ...
        ...
        [BEGIN_PLOT]
            ...
            ...
            ...
            [BEGIN_FIT]
            ...
            ...
            [END_FIT]
        [END_PLOT]
        ...
        ...
        ...
        [BEGIN_PLOT]
            ...
            ...
            ...
            [BEGIN_DATA]
                ...
                ...
                ...
            [END_DATA]
        [END_PLOT]
        ...
        ...
    [END_CANVAS]
```

parameters found inside the `[BEGIN_CANVAS]` header are expected to be entered in the following format:

```
field_name = 'value';
```

additionally these parameters are expected to be entered in the manner described in Section 4.e.ii.

The config file will define one `TCanvas` which will be drawn following the official CMS Style Guide.  For this canvas any number of `TObjects` can be defined and drawn on the canvas.  Right now the `TGraph`, `TGraphErrors`, `TGraph2D`, `TH1F`, and `TH2F` classes are supported. For each of higher level header sections (i.e. `[BEGIN_CANVAS]` and `[BEGIN_PLOT]`) the header parameters should *always* be placed before descending into the next header. Failure to adhere to this convention may lead to undefined behavior or crashes.

Plots can be declared by entering comma separated data in the `[BEGIN_DATA]` header, see Section 4.e.iv.III, or loaded from an input `TFile`, see Section 4.e.iv.II.  Additionally, one or more `TF1` objects can be declared for each plot and drawn on the `TCanvas`.  These `TF1` objects can be loaded from an input `TFile` or fit at the time of the execution.

The `Plotter::ParameterLoaderPlotter` class understands the `#` character to indicate a comment; so it is possible to comment out lines in the *Plot Config* file you create for ease of use. The value of `true` is understood as being from the case-insensitive set `{t, true, 1}` while the value of `false` is understood as being from the case-insensitive set `{f, false, 0}`. The template *plot config* file at the end of this subsection showns an example.

#### 4.e.iv.I HEADER PARAMETERS - CANVAS
The table below describes the allowed input fields and their data types.

Field Name | Type | Description
---------- | ---- | -----------
`Canv_Axis_NDiv` | Two comma separated int's | Defines the number of divisions for a given `TAxis`.  See the `TAxis` [documentation](https://root.cern.ch/doc/master/classTAttAxis.html) for more information.  Between one and three integers can be provided. They are understand as applying to the {X}, {X,Y}, or {X,Y,Z} axes.
`Canv_Dim` | Two comma separated int's | Defines the size of the canvas in pixels.  The first (second) number is for the x (y) direction.
`Canv_DrawOpt` | string | Draw option that will be applied to all plots on this canvas
`Canv_Grid_XY` | Two comma separated bool's | Defines if the grid should be drawn on the canvas.  The first (second) boolean is for the x (y) grid.
`Canv_Latex_Line` | Comma separated sequence: float,float,string | Defines the position of a `TLatex` line and the text in the line.  The first (second) float defines the x (y) position of the line.  Use the `~` character to insert a space between characters as spaces or `\t` in the string will be stripped before it is passed to the `TLatex` class.  Any number of `Canv_Latex_Line` fields may be supplied and they will all be drawn on the `TCanvas`.  Note that "CMS" or "CMS Preliminary" will already be drawn by default.
`Canv_Legend_Dim_X` | Two comma separated floats | Defines the [NDC coordinates](https://root.cern.ch/root/html534/guides/users-guide/Graphics.html#the-coordinate-systems-of-a-pad) of a `TLegend` to be drawn on the pad.  The first (second) float is the X1 (X2) coordinate of the `TLegend`. See the `TLegend` [documentation](https://root.cern.ch/doc/master/classTLegend.html) for more details.
`Canv_Legend_Dim_Y` | Two comma separated floats | As `Canv_Legend_Dim_X` but for the Y coordinates.
`Canv_Legend_Draw` | bool | Setting to true (false) will (not) draw the `TLegend` on the `TCanvas`.
`Canv_Log_XY` | Two comma separated bool's | As `Canv_Grid_XY` but for setting the X & Y axis to logarithmic.
`Canv_Logo_Pos` | int | Indicates the position the CMS logo should be placed.  Possible values are out of frame (0), top-left (11), top-centered (22), or top-right (33).  More details and examples shown [here](https://ghm.web.cern.ch/ghm/plots/).
`Canv_Logo_Prelim` | bool | Defines whether or not a canvas is preliminary (e.g. "CMS Preliminary"). This should be true for all plots unless they are being submitted for CMS CWR (e.g. publication in a peer-review journal).
`Canv_Margin_Bot` | float | Sets the bottom margin of the `TCanvas`.
`Canv_Margin_Lf` | float | Sets the left margin of the `TCanvas`.
`Canv_Margin_Rt` | float | Sets the right margin of the `TCanvas`.
`Canv_Margin_Top` | float | Sets the top margin of the `TCanvas`.
`Canv_Mono_Color` | bool | Determines if the color palette is monocolored (true), e.g. a single shade, or multi-colored (false).  For paper publications this should be false; for presentations/talks this ma be true.
`Canv_N_Axis_X` | int | Placeholder
`Canv_N_Axis_Y` | int | Placeholder
`Canv_Name` | string | `TName` of the created `TCanvas`.
`Canv_Normalize` | bool | Only implemented for `TH1F` objects, if set to true all `TH1F` objects are drawn with an integral of unity.
`Canv_Plot_Type` | string | Defines the type of `TObject` to be plotted on the `TCanvas`. Supported types are from the set {`TGraph`, `TGraph2D`, `TGraphErrors`, `TH1F`, `TH2F`}.
`Canv_Range_X` | Two comma separated int's | Defines the range `[X_min, X_max]` of the X-axis.
`Canv_Range_Y` | Two comma separated int's | As `Canv_Range_X` but for the Y-axis.
`Canv_Range_Z` | Two comma separated int's | As `Canv_Range_X` but for the Z-axis.
`Canv_Title_Offset_X` | float | Defines the offset of the X-axis title
`Canv_Title_Offset_Y` | float | As `Canv_Title_Offset_X` but for the Y-axis.
`Canv_Title_Offset_Z` | float | As `Canv_Title_Offset_X` but for the Z-axis.
`Canv_Title_X` | string | Title (e.g. label) assigned to the X-axis.
`Canv_Title_Y` | string | As `Canv_Title_X` but for the Y-axis.
`Canv_Title_Z` | string | As `Canv_Title_X` but for the Z-axis.
        
#### 4.e.iv.II HEADER PARAMETERS - PLOT
The table below describes the allowed input fields and their data types.

Field Name | Type | Description
---------- | ---- | -----------
`Plot_Color` | TColor | Color assigned to the `TObject` marker, line, and fill color attributes. List of supported colors is defined [here](https://root.cern.ch/doc/master/Rtypes_8h.html#ac31db05c6cb5891c704eae374f6926a8a9e1b2d3dcb56c17a372440853228ad01) and can be view [here](https://root.cern.ch/doc/master/classTColor.html#C02). Note mathematical expressions on `TColor` words is also supported.  For example `kRed+2` or `kBlue-3` will be interpreted correctly.
`Plot_LegEntry` | string | Legend entry for this `TObject`.
`Plot_Line_Size` | float | Line size of the `TObject`.
`Plot_Line_Style` | int | Line [style](https://root.cern.ch/doc/master/classTAttLine.html#L3) of the `TObject`.
`Plot_Marker_Size` | float | Marker size of the `TObject`.
`Plot_Marker_Style` | int | Marker [style](https://root.cern.ch/doc/master/classTAttMarker.html#M2) of the `TObject`
`Plot_Name` | string | `TName` of the `TObject` defined in this `[BEGIN_DATA]` header.  It will either be the `TName` created or loaded from the input `TFile` defined in `Plot_Root_File` found with path `Plot_Root_Path` inside the `TFile`.
`Plot_Root_File` | string | If no `[BEGIN_DATA]` header is supplied for this plot this is the name of the `TFile` from which the desired `TObject` to be plotted is found in.
`Plot_Root_Path` | string | If no `[BEGIN_DATA]` header is supplied for this plot this is the path in `Plot_Root_File` for which `Plot_Name` is found at. Explicitly inside the `TFile` the desired `TObject` is `Plot_Root_Path/Plot_Name`.

#### 4.e.iv.III HEADER PARAMETERS - DATA
Each plot is either found in an input `TFile` using the `Plot_Name`, `Plot_Root_File`, and `Plot_Root_Path` fields or is created from comma separated data found in the `[BEGIN_DATA]` header.  If comma separated data is supplied the `Plot_Root_File` and `Plot_Root_Path` fields are not used. Right now for `Canv_Plot_Type` set to either `TH1F` or `TH2F` the `TFile` input must be used.

If comma separated data is supplied it is done so in this header.  Each line must have between 2 and 4 values.  Each value represents either the x-value, the error on the x-value, the y-value, or the error on the y-value for a given point.  The first line of the `[BEGIN_DATA]` header must be a line consisting of between 2 and 4 strings from the set {`VAR_INDEP`, `VAR_INDEP_ERR`, `VAR_DEP`, `VAR_DEP_ERR`} for the x-value, x-value error, y-value, or y-value error, respectively.  The position of these strings defines the meaning of each entry in all subsequent lines.  For example:

```
VAR_INDEP, VAR_DEP, VAR_INDEP_ERR
```

indicates that the first value of each line is the y-value, the second is the x-value, and the third value is the error on the x-value for a given point. See Section 4.e.iv.VI for an example.

#### 4.e.iv.III HEADER PARAMETERS - FIT
The table below describes the allowed input fields and their data types.

Field Name | Type | Description
---------- | ---- | -----------
`Fit_Color` | TColor | As `Plot_Color` in Section 4.e.iv.II.
`Fit_Formula` | string | As `Fit_Formula` in Section 4.e.ii.IV.
`Fit_LegEntry` | string | As `Plot_LegEntry` in Section 4.e.iv.II.
`Fit_Line_Size` | float | As `Plot_Line_Size` in Section 4.e.iv.II.
`Fit_Line_Style` | int | As `Plot_Line_Style` in Section 4.e.iv.II.
`Fit_Name` | string | `TName` of the `TF1` object.  Either the `TF1` will be created with this `TName` or it will be loaded from `Fit_Root_File` found with path `Fit_Root_Path` inside the `TFile`.
`Fit_Option` | string | If the `TF1` is being loaded from a previous `TFile` with `Fit_Root_File` option this is the *draw* option; if the `TF1` is being fit to one of the `TObjects` on the `TCanvas` this is the *fit* option.
`Fit_Root_File` | string | As `Plot_Root_File` in Section 4.e.iv.II.
`Fit_Root_Path` | string | As `Plot_Root_Path`  in Section 4.e.iv.II.
`Fit_Perform` | bool | If true perform a fit to the `TObject` defined in the `[BEGIN_PLOT]` header this `[BEGIN_FIT]` header is found in; otherwise the `TF1` is just drawn on the `TCanvas`.
`Fit_Range` | Two comma separated strings | As `Fit_Range` in Section 4.e.ii.IV.

#### 4.e.iv.V Configuration Options
The `genericPlotter` executable will create one `TCanvas` for every call of the executable.  The created `TCanvas`, and all plots on it, will follow the official [CMS Style Guide](https://twiki.cern.ch/twiki/bin/viewauth/CMS/Internal/PubGuidelines#Figures_and_tables).  Note the user needs to ensure the additional input configurations provided also conform to the CMS Style Guide (e.g. units on axis labels). However this will provide a "baseline" style to the created canvas to greatly simplify the process for preparing plots for publication.  The `[BEGIN_CANVAS]` header is required.  One or more `TObjects` can be drawn on the canvas.  For each `TObject` you should define a corresponding `[BEGIN_PLOT]`.  Note that all the `TObjects` drawn on the canvas must be of the same type; however `TF1` objects can be drawn on top of the input TObjects or fitted to them. For example you cannot draw both a `TH1F` and a `TGraphErrors` on the canvas simultaneously (you'll need to convert one into the other) but you could fit a `TF1` to a `TGraphErrors`.  The supported types of TObjects are from the set {`TGraph`, `TGraph2D`, `TGraphErrors`, `TH1F`, `TH2F`}.

For the case of `TGraph`, `TGraph2D`, and `TGraphErrors` objects you can choose to provide a `[BEGIN_DATA]` header (see Section 4.e.iv.III) and have the `genericPlotter` executable load comma separated data in and use this data to create the graph or you can have `genericPlotter` load a previously created objects from a `TFile` by supplying the `TName` of the object (`Plot_Name` field), the name of the `TFile` (`Plot_Root_File`), and the path inside the `TFile` the object is located at (`Plot_Root_Path`).

For the case of `TH1F` and `TH2F` objects presently they must be loaded from a previously created `TFile` in the manner described above.

Example plot config files showning `TGraph`, `TGraph2D`, `TGraphErrors`, `TH1F`, and `TH2F` cases are shown below.

#### 4.e.iv.VI Example Config File - TGraph
The following example shows the case where two `TGraph` objects are plotted. Note the `Canv_Plot_Type` in the `[BEGIN_CANVAS]` header is set to `TGraph`. Here one `TGraph` is supplied via comma separated data while the other is loaded from an input `TFile`.

```
[BEGIN_CANVAS]
    Canv_Axis_NDiv = '510,510'; #X, Y
    Canv_Dim = '1000,1000'; #X, Y
    Canv_DrawOpt = 'APE1';
    Canv_Grid_XY = 'false,false'; #X, Y
    Canv_Latex_Line = '0.19,0.75, Ar/CO_{2}~=~#left(70/30#right)'; #X_NDC, Y_NDC, String
    Canv_Latex_Line = '0.19,0.70, X-Ray#left(Ag#right);40~kV;5~#muA'; #X_NDC, Y_NDC, String
    Canv_Latex_Line = '0.19,0.65, i#eta~=~4;i#phi~=~2'; #X_NDC, Y_NDC, String
    Canv_Legend_Dim_X = '0.5,0.95'; X_NDC_1, X_NDC_2
    Canv_Legend_Dim_Y = '0.14,0.40'; Y_NDC_1, Y_NDC_2
    Canv_Legend_Draw = 'true';
    Canv_Log_XY = 'false,true'; #X, Y
    Canv_Logo_Pos = '11'; #0, 11, 22, 33
    Canv_Logo_Prelim = 'true';
    Canv_Margin_Top = '0.08';
    Canv_Margin_Bot = '0.14';
    Canv_Margin_Lf = '0.15';
    Canv_Margin_Rt = '0.04';
    Canv_Name = 'GainComp';
    Canv_Plot_Type = 'TGraph';
    Canv_Range_X = '500,800'; #X1, X2
    Canv_Range_Y = '10,30000'; #Y1, Y2
    Canv_Title_Offset_X = '1.0';
    Canv_Title_Offset_Y = '1.2';
    Canv_Title_X = 'Divider Current #left(#muA#right)';
    Canv_Title_Y = 'Effective Gain';
    [BEGIN_PLOT]
        Plot_Color = 'kBlack';
        Plot_LegEntry = 'GE1/1-VII-L-CERN-0004';
        Plot_Line_Size = '1';
        Plot_Line_Style = '1';
        Plot_Marker_Size = '1';
        Plot_Marker_Style = '22';
        Plot_Name = 'g_GE1/1-VII-L-CERN-0004_EffGain';
        [BEGIN_DATA]
            #NOTE:  Copying/pasting data from excel may not have the desired effect.
            #	Have noticed that a "newline" character (which is invisible) is
            #	not present in some copy/past actions, if your data does not load
            #	properly consider this as a potential cause.
            VAR_INDEP,VAR_DEP
            700,24850.98774
            690,16972.77181
            680,11719.64474
            670,8115.337175
            660,5608.074856
            650,3882.143995
            640,2706.058443
            630,1909.212518
            620,1359.699531
            610,964.186757
            600,698.2227425
            590,516.9014257
            580,359.4035559
            570,264.6152389
            560,192.4948981
            550,140.1252986
        [END_DATA]
    [END_PLOT]
[END_CANVAS]
```

#### 4.e.iv.VII Example Config File - TGraph2D
The following example shows the case where a `TGraph2D` object is plotted.  Note the `Canv_Plot_Type` in the `[BEGIN_CANVAS]` header is set to `TGraph2D`. Here the `TGraph2D` is loaded from an input `TFile`.

```
[BEGIN_CANVAS]
    Canv_Axis_NDiv = '507,510'; #X, Y
    Canv_Dim = '1000,1000'; #X, Y
    Canv_DrawOpt = 'TRI2Z';
    Canv_Grid_XY = 'false,false'; #X, Y
    #Canv_Latex_Line = '0.55,0.97, GE1/1-VII-L-CERN-0002'; #X_NDC, Y_NDC, String
    Canv_Legend_Dim_X = '0.5,0.95'; X_NDC_1, X_NDC_2
    Canv_Legend_Dim_Y = '0.14,0.40'; Y_NDC_1, Y_NDC_2
    Canv_Legend_Draw = 'false';
    Canv_Log_XY = 'false,false'; #X, Y
    Canv_Logo_Pos = '0';
    Canv_Logo_Prelim = 'true';
    Canv_Margin_Top = '0.08';
    Canv_Margin_Bot = '0.14';
    Canv_Margin_Lf = '0.19';
    Canv_Margin_Rt = '0.24';
    Canv_Mono_Color = 'false';
    Canv_Name = 'GE11-VII-L-CERN-0002_ResponseMap_Normalized'';
    Canv_N_Axis_X = '1';
    Canv_N_Axis_Y = '1';
    Canv_Plot_Type = 'TGraph2D';
    Canv_Range_Z = '0.5,1.5'; #Y1, Y2
    Canv_Title_Offset_X = '1.2';
    Canv_Title_Offset_Y = '1.65';
    Canv_Title_Offset_Z = '1.32';
    Canv_Title_X = 'Width #left(mm#right)';
    Canv_Title_Y = 'Height #left(mm#right)';
    Canv_Title_Z = '#splitline{Normalized Response}{#bf{GE1/1-VII-L-CERN-0002}}';
    [BEGIN_PLOT]
        Plot_Name = 'g2D_Detector_ResponseFitPkPosNormalized_AllEta';
        Plot_Root_File = '/path/to/file/GE11-VII-L-CERN-0002_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_580uA_10826kEvt_AnaWithFits.root';
        Plot_Root_Path = '/path/to/Plot_Name/in/Plot_Root_File/';
    [END_PLOT]
[END_CANVAS]
```

Note the `Plot_Root_File` should be the PFN of the `TFile` and `Plot_Root_Path` should be the physical path to the `TGraph2D` object, e.g. `g2D_Detector_ResponseFitPkPosNormalized_AllEta` in this example.

#### 4.e.iv.VIII Example Config File - TGraphErrors
The following example shows the case where two `TGraphError` objects are plotted. Note the `Canv_Plot_Type` in the `[BEGIN_CANVAS]` header is set to `TGraphErrors`. Here one `TGraphError` is supplied via comma separated data while the other is loaded from an input `TFile`.

```
[BEGIN_CANVAS]
    Canv_Axis_NDiv = '510,510'; #X, Y
    Canv_Dim = '1000,1000'; #X, Y
    Canv_DrawOpt = 'APE1';
    Canv_Grid_XY = 'false,false'; #X, Y
    Canv_Latex_Line = '0.19,0.75, Ar/CO_{2}~=~#left(70/30#right)'; #X_NDC, Y_NDC, String
    Canv_Latex_Line = '0.19,0.70, X-Ray#left(Ag#right);40~kV;5~#muA'; #X_NDC, Y_NDC, String
    Canv_Latex_Line = '0.19,0.65, i#eta~=~4;i#phi~=~2'; #X_NDC, Y_NDC, String
    Canv_Legend_Dim_X = '0.5,0.95'; X_NDC_1, X_NDC_2
    Canv_Legend_Dim_Y = '0.14,0.40'; Y_NDC_1, Y_NDC_2
    Canv_Legend_Draw = 'true';
    Canv_Log_XY = 'false,true'; #X, Y
    Canv_Logo_Pos = '11'; #0, 11, 22, 33
    Canv_Logo_Prelim = 'true';
    Canv_Margin_Top = '0.08';
    Canv_Margin_Bot = '0.14';
    Canv_Margin_Lf = '0.15';
    Canv_Margin_Rt = '0.04';
    Canv_Name = 'GainComp';
    Canv_Plot_Type = 'TGraphErrors';
    Canv_Range_X = '500,800'; #X1, X2
    Canv_Range_Y = '10,30000'; #Y1, Y2
    Canv_Title_Offset_X = '1.0';
    Canv_Title_Offset_Y = '1.2';
    Canv_Title_X = 'Divider Current #left(#muA#right)';
    Canv_Title_Y = 'Effective Gain';
    [BEGIN_PLOT]
        Plot_Color = 'kBlack';
        Plot_LegEntry = 'GE1/1-VII-L-CERN-0004';
        Plot_Line_Size = '1';
        Plot_Line_Style = '1';
        Plot_Marker_Size = '1';
        Plot_Marker_Style = '22';
        Plot_Name = 'g_GE1/1-VII-L-CERN-0004_EffGain';
        [BEGIN_DATA]
            #NOTE:  Copying/pasting data from excel may not have the desired effect.
            #	Have noticed that a "newline" character (which is invisible) is
            #	not present in some copy/past actions, if your data does not load
            #	properly consider this as a potential cause.
            VAR_INDEP,VAR_DEP,VAR_DEP_ERR
            700,24850.98774,967.6738646
            690,16972.77181,733.8815033
            680,11719.64474,482.4250694
            670,8115.337175,323.215912
            660,5608.074856,223.2476327
            650,3882.143995,157.3363848
            640,2706.058443,111.6052345
            630,1909.212518,87.83123466
            620,1359.699531,63.74477198
            610,964.186757,47.84551633
            600,698.2227425,41.03127041
            590,516.9014257,32.91787275
            580,359.4035559,26.97947809
            570,264.6152389,22.77914941
            560,192.4948981,21.47259055
            550,140.1252986,20.45959455
        [END_DATA]
    [END_PLOT]
    [BEGIN_PLOT]
        Plot_Color = 'kRed+1';
        Plot_LegEntry = 'GE1/1-VII-L-CERN-0003';
        Plot_Line_Size = '1';
        Plot_Line_Style = '1';
        Plot_Marker_Size = '1';
        Plot_Marker_Style = '23';
        Plot_Name = 'g_GE1/1-VII-L-CERN-0003_EffGain';
        Plot_Root_File = '/path/to/file/GE11-VII-L-CERN-0003_EffGain.root';
        Plot_Root_Path = '/path/to/Plot_Name/in/Plot_Root_File/';
    [END_PLOT]
[END_CANVAS]
```

Note the `Plot_Root_File` should be the PFN of the `TFile` and `Plot_Root_Path` should be the physical path to the `TGraphErrors` object `g_GE11-VII-L-CERN-0003_EffGain`.

        # 4.e.iv.IX Example Config File - TH1F
        # --------------------------------------------------------

        The following example shows the case where a TH1F object is plotted.  Note the Canv_Plot_Type
        in the [BEGIN_CANVAS] header is set to TH1F. Here the TH1F is loaded from an input TFile.

            [BEGIN_CANVAS]
                Canv_Axis_NDiv = '510,510'; #X, Y
                Canv_Dim = '1000,1000'; #X, Y
                Canv_DrawOpt = 'E1';
                Canv_Grid_XY = 'false,false'; #X, Y
                Canv_Latex_Line = '0.19,0.75, GE1/1-VII-L-CERN-0002'; #X_NDC, Y_NDC, String
                Canv_Legend_Dim_X = '0.5,0.95'; X_NDC_1, X_NDC_2
                Canv_Legend_Dim_Y = '0.14,0.40'; Y_NDC_1, Y_NDC_2
                Canv_Legend_Draw = 'false';
                Canv_Log_XY = 'false,false'; #X, Y
                Canv_Logo_Pos = '11';
                Canv_Logo_Prelim = 'true';
                Canv_Margin_Top = '0.08';
                Canv_Margin_Bot = '0.12';
                Canv_Margin_Lf = '0.12';
                Canv_Margin_Rt = '0.04';
                Canv_Name = 'GE11-VII-L-CERN-0002_RespFitPkPosDataset';
                Canv_Plot_Type = 'TH1F';
                Canv_Range_X = '0,2500'; #X1, X2
                Canv_Range_Y = '0,150'; #Y1, Y2
                Canv_Title_Offset_X = '1.2';
                Canv_Title_Offset_Y = '1.65';
                Canv_Title_X = 'Fitted Cu Fluorescence Peak Position #left(ADC#right)';
                Canv_Title_Y = 'Entries';
                [BEGIN_PLOT]
                    Plot_Color = 'kBlack';
                    Plot_LegEntry = 'Data';
                    Plot_Name = 'h_Summary_ResponseFitPkPosDataset';
                    Plot_Root_File = 'GE11-VII-L-CERN-0002_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_580uA_10826kEvt_AnaWithFits.root';
                    Plot_Root_Path = 'Summary/';
                    [BEGIN_FIT]
                        Fit_Color = 'kRed';
                        Fit_LegEntry = 'Fit';
                        Fit_Line_Size = '1';
                        Fit_Line_Style = '1';
                        Fit_Name = 'fit_Summary_ResponseFitPkPosDataset';
                        Fit_Root_File = 'GE11-VII-L-CERN-0002_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_580uA_10826kEvt_AnaWithFits.root';
                        Fit_Root_Path = 'Summary/';
                    [END_FIT]
                [END_PLOT]
            [END_CANVAS]

        Note the Plot_Root_File should be the PFN of the TFile and Plot_Root_Path should be
        the physical path to the TH1F object "h_Summary_ResponseFitPkPosDataset".

        # 4.e.iv.X  Example Config File - TH2F
        # --------------------------------------------------------

        The following example shows the case where a TH2F object is plotted.  Note the Canv_Plot_Type
        in the [BEGIN_CANVAS] header is set to TH2F. Here the TH2F is loaded from an input TFile.

        [BEGIN_CANVAS]
            Canv_Axis_NDiv = '507,510'; #X, Y
            Canv_Dim = '1000,1000'; #X, Y
            Canv_DrawOpt = 'COLZTEXT';
            Canv_Grid_XY = 'false,false'; #X, Y
            Canv_Legend_Dim_X = '0.5,0.95'; X_NDC_1, X_NDC_2
            Canv_Legend_Dim_Y = '0.14,0.40'; Y_NDC_1, Y_NDC_2
            Canv_Legend_Draw = 'false';
            Canv_Log_XY = 'false,false'; #X, Y
            Canv_Logo_Pos = '0';
            Canv_Logo_Prelim = 'true';
            Canv_Margin_Top = '0.08';
            Canv_Margin_Bot = '0.14';
            Canv_Margin_Lf = '0.13';
            Canv_Margin_Rt = '0.19';
            Canv_Mono_Color = 'false';
            Canv_Name = 'GE11-VII-L-CERN-0001_FitSuccess_RGB';
            Canv_Plot_Type = 'TH2F';
            Canv_Range_Y = '1,9'; #Y1, Y2
            Canv_Range_Z = '0,1'; #Y1, Y2
            Canv_Title_Offset_X = '1.2';
            Canv_Title_Offset_Y = '1.2';
            Canv_Title_Offset_Z = '1.65';
            Canv_Title_X = 'Detector #it{i#phi} index';
            Canv_Title_Y = 'Detector #it{i#eta} index';
            Canv_Title_Z = 'GE1/1-VII-L-CERN-0001 Fit Success';
            [BEGIN_PLOT]
                Plot_Name = 'h_Summary_FitSuccess';
                Plot_Root_File = '/afs/cern.ch/user/d/dorney/scratch0/CMS_GEM/CMS_GEM_Analysis_Framework/data/sliceTestAna/QC5_Resp_Uni/GE11-VII-L-CERN-0001_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_588uA_TimeCorr_DPGGeo_AnaWithFits.root';
                Plot_Root_Path = 'Summary';
            [END_PLOT]
        [END_CANVAS]

        Note the Plot_Root_File should be the PFN of the TFile and Plot_Root_Path should be
        the physical path to the TH2F object "h_Summary_FitSuccess".

        # 4.e.v Efficiency Predictor Config File
        # --------------------------------------------------------

        Unlike most other config files used in the framewok the Efficiency Predictor Config File does
        not use a nested header style to load in information.  Instead each line consists of tab
        delimited data, the field name appears first, followed by a tab, and then the value to be assigned
        to the field.  For example:

            field_name\tValue

        Where the tab '\t' character has been shown explicitly.  The possible field names are shown in the
        following subsections

        # 4.e.v.I   PARAMETERS - Input Files
        # --------------------------------------------------------

        The table below describes the allowed input fields and their data types.

        The following parameters are supported:
        #		<FIELD>                             <DATA TYPE, DESCRIPTION>

                File_Framework_Output               string, PFN of the input framework out file produced in
                                                    the QC5_Resp_Uni measurement

                File_ClustQ_Mean                    string, PFN of a tab deliminted file storing cluster
                                                    charge landau data collected with triple-GEM detectors.
                                                    The data is ordered as V_Drift, Cluster Size, and the
                                                    Landau mean parameter.  Note the first two lines of this
                                                    file are assumed to consist of column headers and units
                                                    and are thus skipped.

                File_ClustQ_MPV                     string, as File_ClustQ_Mean but for Landau MPV parameter

                File_ClustQ_Sigma                   string, as File_ClustQ_Mean but for Landau Sigma (e.g.
                                                    scale) parameter

                File_ClustSize                      string, PFN of a TFile storing a TF1 object which gives
                                                    MIP cluster size parameterized in terms of triple-GEM
                                                    detector gain.

                File_DUT_Mapping_Geo                string, PFN of the mapping config file used to create
                                                    the given File_Framework_Output

                File_DUT_Mapping_VFATPos2iEtaiPhi   string, PFN of a tab deliminted mapping file which gives
                                                    the correspondance between detector (ieta,iphi) coordinate
                                                    to VFAT position.  Note the first line of this file is
                                                    assume to consist of column headers and is thus skipped

                File_DUT_SCurveData                 string, PFN of a TFile storing the scurveFitTree TTree
                                                    object which contains the analyzed S-Curve data recorded
                                                    with a detector instrumented with the v2b GEM Electronics.

                File_Output                         string, PFN of the output file to be produced by
                                                    calling computeEffCurves.py

        # 4.e.v.II  PARAMETERS - Device Under Test (DUT)
        # --------------------------------------------------------

        The table below describes the allowed input fields and their data types.

        The following parameters are supported:
        #		<FIELD>                             <DATA TYPE, DESCRIPTION>

                DUT_GAIN_P0                         float, for the detector corresponding to the QC5 data
                                                    stored in File_Framework_Output, this is value of p0 in
                                                    G(x) = exp(p0*x+p1); here x is either divider current
                                                    or V_Drift

                DUT_GAIN_P0_Err                     float, error on DUT_GAIN_P0

                DUT_GAIN_P1                         float, as DUT_GAIN_P0 but for p1 in G(x) = exp(p0*x+p1)

                DUT_GAIN_P1_Err                     float, error on DUT_GAIN_P1

                DUT_iEta_Clust_Size_Norm            int, iEta index corresponding to (ieta,iphi) sector that
                                                    data in File_CluseSize was obtained from

                DUT_iEta_QC5_Gain_Cal               int, iEta index corresponding to (ieta,iphi) sector that
                                                    QC5_Gain_Cal was performed in

                DUT_iPhi_Clust_Size_Norm            int, as DUT_iEta_Clust_Size_Norm but for iPhi index

                DUT_iPhi_QC5_Gain_Cal               int, as DUT_iEta_QC5_Gain_Cal but for iPhi index

                DUT_Num_Sim_Pts_Per_RO              int, number of Toy MC events to simulate for each slice
                                                    of the detector found in File_Framework_Output

                DUT_QC5_Resp_Uni_HVPt               float, HV value at which the QC5_Resp_Uni measurement was
                                                    performed at (e.g. HV value used to obtain File_Framework_Output).
                                                    Note the HV observable here, either divider current or
                                                    V_Drift, should match the observable that was used to
                                                    parameterize the Gain curve.

                DUT_Serial_Number                   string, value assigned to the Detector_Name field in the
                                                    input run config file used to create the given
                                                    File_Framework_Output.  Note: "Detector" should be used
                                                    if the user did not supply a value to this field in the
                                                    run config file when generating the File_Framework_Output

                DUT_Slice_Granularity               int, value assigned to the Uniformity_Granularity field
                                                    in the input analysis config file used to create the
                                                    given File_Framework_Output

        # 4.e.v.III PARAMETERS - Cluster Charge
        # --------------------------------------------------------

        The table below describes the allowed input fields and their data types.

        The following parameters are supported:
        #		<FIELD>                             <DATA TYPE, DESCRIPTION>

                Det_ClustQ_Serial_Number            string, serial number of the detector used to obtain
                                                    the data found in File_ClustQ_Mean, File_ClustQ_MPV,
                                                    and File_ClustQ_Sigma files

                Det_ClustQ_GAIN_P0                  float, for Det_ClustQ_Serial_Number this is value of p0 in
                                                    G(V_Drift) = exp(p0*V_Drift+p1)

                Det_ClustQ_GAIN_P0_Err              float, error on Det_ClustQ_GAIN_P0

                Det_ClustQ_GAIN_P1                  float, as Det_ClustQ_GAIN_P0 but for p1 in
                                                    G(V_Drift) = exp(p0*V_Drift+p1)

                Det_ClustQ_GAIN_P1_Err              float, error on Det_ClustQ_GAIN_P1

        # 4.e.v.IV  PARAMETERS - Cluster Size
        # --------------------------------------------------------

        The table below describes the allowed input fields and their data types.

        The following parameters are supported:
        #		<FIELD>                             <DATA TYPE, DESCRIPTION>

                Det_ClustSize_Serial_Number         string, serial number of the detecotr used to obtain the
                                                    data found in File_ClustSize

                Det_ClustSize_TF1_TName             string, name of the TF1 object in File_ClustSize which
                                                    parameterizes MIP cluster size parameterized in terms of
                                                    triple-GEM detector gain.

#### 4.e.v.V   PARAMETERS - Efficiency Info
#### --------------------------------------------------------
The table below describes the allowed input fields and their data types.

The following parameters are supported:
FIELD | DATA TYPE | DESCRIPTION
----- | --------- | -----------
Eff_HVPt_List | comma separated list of ints | list of HV Pt's the efficiency should be predicted at.

## 4.f. Output File - Analysis Mode
## --------------------------------------------------------
The framework will produce a number of output `ROOT` files and text files depending on the configuration used.  When `Output_Individual = true` one `ROOT` file and text file will be produced per input file.  Otherwise a single `ROOT` and text file will be produced which represents the aggregate of the input file(s) analyzed

The output (text) `ROOT` file is described in Section (4.f.ii) 4.f.i.

### 4.f.i Output ROOT File - Analysis Mode
### --------------------------------------------------------
The output `ROOT` file produced by classes inheriting from `AnalyzeResponseUniformity` will contain the `TObjects` described in Sections 4.b.i, 4.b.iii, and 4.d.ii.  The output file will have a repeating file structure.  For each `SectorEta` defined (i.e. "**DET**" line in the *mapping config file*) there will be one `TDirectory` named `SectorEtaX` where `X` is an integer.  Those `TObject`'s stored in the `Uniformity::SectorEta` struct will be stored directly in this `SectorEtaX` TDirectory; and obviously they will represent only distributions from that iEta value.  The `TName`'s for each `TObject` here will include the string `_iEtaX_` to ensure they are unique.

Within each `SectorEtaX` folder will be `nbConnect` number of `TDirectory`'s labeled `SectorPhiY` for `Y = {1, ... , nbConnect}`.  Similarly to the above, the `TObject`'s stored in the `Uniformity::SectorPhi` struct will be stored directly in this `SectorPhiY` TDirectory; they will represent only distributions from this (iEta, iPhi) value.  Again, the `TName`'s for each `TObject` here will include the string `_iEtaXiPhiY_` to ensure they are unique.

Within each `SectorPhiY` folder there will exist `AnalysisSetupUniformity::iUniformityGranularity` number of `TDirectory`'s labeled `SliceZ` where `Z` is an integer from 1 to `AnalysisSetupUniformity::iUniformityGranularity`.  Similarly, the `TObject`'s stored in `Uniformity::SectorSlice` struct will be stored directly in this `SliceZ` TDirectory; they will only represent distributions from this (iEta, iPhi, Slice) value.  Again, the `TName`'s for each `TObject` here will include the string `_iEtaXiPhiYSliceZ_` to ensure tehy are unique.

In general all `TObjcets` stored in the output `ROOT` file will follow a convention for their `TNames`. For one dimensional `TObjects` we use the following convention/regular expression:

```   
 <TypePrefix>_<Coordinate>_<ObservableNameX>
```

For two dimensional `TObjects` we use the following convention:

```    
<TypePrefix>_<Coordinate>_<ObservableNameY>_v_<ObservableNameX>
```

Where: the **TypePrefix** is described in Section 4.g.i (e.g. for `TH1F` objcets it is `h`); the **Coordinate** is the `(iEta,iPhi,iSlice)` point of the histogram as described above; the **ObservableNameX**, and **ObservableNameY** fields are respectively what is plotted on the X & Y access of the `TObject`.

One top level `TDirectory` named **Summary** will also exist.  This folder will store a set of histograms for each cluster/hit observable.  The contents of these histograms is simply the sum of the corresponding `SectorEtaX` histograms (e.g. `TH1::Add()` method).

Additionally the `VisualizeUniformity` class places additional `TObjects` (e.g. `TCanvas`, `TMultiGraph`, etc...) to assist the analyst in making the "pass/fail" statement.  These are desribed below.

#### 4.f.i.I "Segmented" Plots Stored in "Summary" folder
#### --------------------------------------------------------

Several `TCanvas` objects with `TNames` of the form:

```    
canv_<Detector_Name>_<Observable>_<ReadoutLevel>_Segmented
```

will be stored in the folder.  Here the **Detector_Name** is the parameter defined in the given `configRun.cfg` file; **Observable** comes from the set {`ClustADC`, `ClustMulti`, `ClustPos`, `ClustSize`, `ClustTime`, `HitADC`, `HitMulti`, `HitPos`, `HitTime`}; and **ReadoutLevel** comes from the set {`AllEta`, `AllPhi`}.

These will show a `TCanvas` with an array of `TPads` placed in a columns-by-row grid of (3-by-8) 2-by-4 grid for (AllPhi) AllEta case.  Each `TPad` will have iEta index written in the upper left corner of the pad and have the corresponding `TObject` from this **ReadoutLevel** (e.g. iEta or iPhi) drawn on the pad.

#### 4.f.ii.II "Dataset" Plots Stored in "Summary" folder
#### --------------------------------------------------------

Several `TCanvas` objects with `TNames` of the form:

```    
canv_<Detector_Name>_<Observable>Dataset_AllEta
```

will be stored in the folder along with matching `TH1F` objects for each `TCanvas` with the `TName` of the form:

```    
h_Summary_<Observable>Dataset
```

Here the **Detector_Name** is the parameter defined in the given `configRun.cfg` file and **Observable** comes from the set {`ResponseFitPkPos`, `ResponseFitPkRes`}, with expansions possible if requested.

The x-axis will be the **Observable** in question (e.g. for `ResponseFitPkPos` this will be the cluster ADC of the peak determined from the fit).  The Y-axis will be counts.  These canvases show the distribution of the observable in question over the entire detector.  The `TH1F` in question will always have the bin range `[Avg - 5 * StdDev, Avg + 5 * StdDev)` with a bin width of `0.25 * StdDev`.  Here `Avg` is the average of the dataset and `StdDev` is the dataset's standard deviation.  This `TH1F` will also be automatically fit with both a Gaussian and a Landau distribution.  The Fit with the lowest Normalized Chi2 value will be kept.  The whose mean (MPV) and sigma (scale) parameter of the stored Gaussian (Landau) will be written on the `TPad`.  The percent error of the dataset, defined as sigma / mean (scale / MPV) from the Gaussian (Landua), will also be displayed on the `TPad`.  This offers an "at a glance" look at the total distribution for a given observable and may help understand an immediate pass/fail condition.

Additionally there will be one `TCanvas` and `TGraphErrors` object with "**Shifted**" in it's name. This is identical to the objects without the "Shifted" string in their names but here the mean position of the plot has been shifted to 0.  This allows you to better compare across detectors when you are interested in the spread of the distribution rather than the mean.  Here two points at (+/-2000,0) have been added to allow a greater range of drawing the x-axis with `genericPlotter` or some custom code of your choice.

#### 4.f.i.III 1D Fit Summary Plots Stored in "Summary" folder
#### --------------------------------------------------------
Several `TCanvas` objects with `TNames` of the form:

```
canv_<Detector_Name>_<FitObservable>_AllEta
```

will be stored in the folder along with matching `TMultiGraph` objects for each `TCanvas` with the `TName` of the form:

```   
mgraph_<Detector_Name>_<FitObservable>_AllEta
```

Here the **Detector_Name** is the parameter defined in the given `configRun.cfg` file. The **FitObservable** is from the set {`ResponseFitChi2`, `ResponseFitPkPos`, `ResponseFitPkRes`} for the normalized Chi2 value of the fit, determined peak position, and determined peak resolution (see Section 4.e.ii.IV for details), respectively.

#### 4.f.i.IV 2D Fit Trapezoidal Map Plots Stored in "Summary" folder
#### --------------------------------------------------------
Several `TCanvas` objects with `TNames` of the form:

```
canv_<Detector_Name>_<FitObservable>2D_AllEta
```

will be stored in the folder along with matching `TGraph2D` objects for each `TCanvas` with the
`TName` of the form:

```    
g2D_<Detector_Name>_<FitObservable>_AllEta
```

Note in the case of the `TCanvas` a *"2D"* string is placed in the `TName` to distinguish it from the *1D* case.
Here the **Detector_Name** is the parameter defined in the given `configRun.cfg` file. The **FitObservable**
is from the set {`ResponseFitPkPos`, `ResponseFitPkPosNormalized`, `ResponseFitPkRes`,
`ResponseFitPkResNormalized`}.  For the *"Normalized"* cases the z-axis at every point will be the point
divided by the mean of the dataset formed by all points of the **FitObservable** (e.g. `z / z_avg`);

These plots may take some time to load.  This is due to the rendering that is done by `ROOT`; be patient.  Consider transfering the file to your local machine if it is not already.  Once they load the plots will show a 3D plot of the detector.  The xy-plane will be the trapezoidal active area of the detector and the Z-axis will be the **FitObservable**.

### 4.f.ii Output ROOT File - Comparison Mode
### --------------------------------------------------------
Here the output `ROOT` file is produced by classes inheriting from `VisualizeComparison`.  The `ROOT` file will contain a `TDirectory` whose `TName` is equal to the value of the **Input_Identifier** field at the time of execution.  Inside this folder there will be a `TDirectory` whose `TName` is equal to the value of the "**Obs_Name** field at the time of execution.

If you have the **Output_File_Option** field equal to `UPDATE` then rather than over-writing the **Output_File_Name** `TFile` everytime it will simply add TDirectories (or sub-TDirectories) to the file.  This is perfect if you want to compare multiple **Obs_Name** distributions for the same **Input_Identifier** value.  Additionally you could have one `TFile` store several different **Input_Identifier** top-level TDirectories each with multipler **Obs_Name** sub-directories.

In each **Obs_Name** sub directory you will find a `TH1F` object from each of the input files you considered on execution.  The `TNames` of these `TH1F` objects will be equivalent to what they are in their original `TFile` but they will be appended with the **Input_Identifier** field specific to that `TFile`.  These `TH1F` objects will automatically be drawn on a `TCanvas`, colored, and placed in a `TLegend` (also drawn on the `TCanvas`).  The `TName` of this `TCanvas` will follow the convention:

```
canv_<Input_Identifier>_<Obs_Name>
```

Example if you have **Input_Identifier** equal `ClustTime` and you are running over the following list of input files:

```
/some/file/path/GE11-VII-L-CERN-0002_Summary_Physics_RandTrig_AgXRay40kV100uA_580uA_15004kEvt_ClustTime1to30_ClustSize1to20_Ana.root
/some/file/path/GE11-VII-L-CERN-0002_Summary_Physics_RandTrig_AgXRay40kV100uA_580uA_15004kEvt_ClustTime6to27_ClustSize1to20_Ana.root
```
The **Obs_Name** sub directory will contain two `TH1F` objects with their regular `TNames` appended with `_ClustTime1to30`
and `ClustTime6to27`.  The `TCanvas` they are drawn on will be named `canv_ClustSize_<Obs_Name>`.

### 4.f.iii Output ROOT File - genericPlotter
### --------------------------------------------------------
Here an output `ROOT` file is produced by classes inheriting from `PlotterGeneric` with the filename `plotterOutput.root`.  The `ROOT` file will contain the produced `TCanvas` and all `TObjects` that were declared in `[BEGIN_PLOT]` headers and drawn on the canvas. The `TName` of the produce `TCanvas` will follow the convention:

```
<Canv_Name>_<Canv_Dim X component>_<Canv_Dim Y component>_<Canv_Logo_Pos Idx>_<Canv_Logo_Prelim>_<Canv_Logo_Pos>
```

Here **Canv_Name** is the value of the `Canv_Name` field; **Canv_Dim X & Y component** are the x and y-values given in the `Canv_Dim` field; **Canv_Logo_Pos Idx** is the value of the `Canv_Logo_Pos` field; **Canv_Logo_Prelim** will read `prelim` if **Canv_Logo_Prelim** is set to `true` or be omitted (along with the next underscore) if **Canv_Logo_Prelim** is set to `false`; **Canv_Logo_Pos** will be from the set {`out`, `right`, `center`, `left`} to indicate the position of the CMS logo.  All fields here are found in the *plot config file*.

For example if:
- `Canv_Name = GE11-VII-L-CERN-0002_ResponseMap_Normalized`, 
- `Canv_Dim = 1000,1000`,
- `Canv_Logo_Pos = 0`, and 
- `Canv_Logo_Prelim = true` 

then the produced `TCanvas` would have a `TName` of:

```
GE11-VII-L-CERN-0002_ResponseMap_Normalized_1000_1000_0_prelim_out
```

The `TObjects` drawn on the `TCanvas` declared in `[BEGIN_PLOT]` headers will have the value of the `Plot_Name` field set to their `TName`.

Since the style defined by `genericPlotter` may not persist in the created `TObjects` once they have been saved to the output `TFile` and loaded again in `ROOT` two image files will also be produced.  In the working directory you will also find `Canvas_TName.C`, `*.eps`, `*.pdf`, and `*.png` files.  These files should be used for plot approval in publication since they are gauranteed to have the style settings created by `genericPlotter`.

### 4.f.iv Output Text File
### --------------------------------------------------------
An output text file will be created that will show in tabular form a table which looks like:

```
Cut_HitAdc_Min = 60
iEta	iPhi	iStrip
1       1       64
1       1       65
1       2       64
1       3       64
...     ...     ...
...     ...     ...
7       1       128
7       2       1
8       3       64
8       2       1
```

The first row will show `Cut_HitAdc_Min`, the minimum ADC cut on hits, applied during the analysis.

A strip is considered dead if there are no entries in the corresponding bin of hit position histogram at the iEta level.  The algorithm will transpose this strip number from iEta numbering, e.g. [1,384], to iPhi numbering [1,128]. This should allow the user to easily identify which strip on a connector is reported as dead.

Note that having the `Cut_HitAdc_Min` *too high* may cause strips to be reported as dead.  Also when identifying dead strips it is important to understand if the problem is a dead strip on the detector or a dead channel on the front end used to readout the sector.

### 4.f.v Output ROOT File - Gain Map
### --------------------------------------------------------
This file will have a series of TDirectories of the form:

```
GainMap_HVPt<X>
```

Where **X** corresponds to the HV values given to parameters {`hvPoint`,`hvlist`} either as command line arguments or entries in the provided *Summary File*.  Additionally there will be one `TDirectory` named `Summary`.

Inside each of the `GainMap_HVPt<X>` TDirectories you will find `TCanvas` objects whose `TNames` follow the form:

```
canv_<name>_<Observable>_AllEta_<hvPoint>
```

Where: **name** is the string given to the `--name` argument at command line or provided in the *Summary File*, **Observable** is from the set {`EffGain`, `PD`}, and **hvPoint** is either the value given to the `hvPoint` argument or one of the values given to the `hvlist` argument, or their respective entries in the *Summary File*.  Note that `PD` corresponds to _discharge probability induced by alpha-particles_; and is only calculated for HV values found in the `hvlist` argument.

Additionally, inside each of the `GainMap_HVPt<X>` TDirectories you will find `TGraph2D` objects whose `TNames` follow the form:

```
g2D_<name>_<Observable>_AllEta_<hvPoint>
```

Where the **name**, **Observable**, and **hvPoint** are as above.

Inside the `Summary` TDirectory you will find `TCanvas` objects whose `TNames` follow the form:

```
canv_<name>_<Observable>
```

Where: **name** is as given above and **Observable** is from the set {`EffGainAvg`, `PDAvg`}.  Additionally you will find a set of `TGraphError` objects whose `TNames` follow the form:

```
g_<name>_<Observable>
```

Where: **name** is as given above and **Observable** is from the set {`EffGainAvg`, `EffGainMax`, `EffGainMin`, `PDAvg`, `PDMax`, and `PDMin`}.  

### 4.f.vi Output ROOT File - Efficiency Map
### --------------------------------------------------------

# 5. Troubleshooting
# ========================================================
    If you run into trouble please double check this file to ensure you are using the repository correctly.  If you still are running into trouble please navigate too the [issue tab](https://github.com/bdorney/CMS_GEM_Analysis_Framework/issues) of the repository and post an issue following the instructions included in the template.
