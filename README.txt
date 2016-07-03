# CMS_GEM_Analysis_Framework
# ========================================================

	Software tools for the analysis of experimental data collected by the CMS GEM community

	Designed to work on lxplus running slc6 with ROOT version 6.00.02 and g++ version 4.8.4

    Instructions below assume the user is using a sh/bash/zsh shell.  Scripts for csh/tsch
    have not been implemented yet.

    This README serves as the single point of entry for all installation and usage instructions.
    Significant documentation for users (developers not yet!) has also been included.

    If you are having problems please consult Section 5 "Known & Outstanding Issues" first.  The
    problem and solution may already be present.  If you do not find your issue please navigate to:

        https://github.com/bdorney/CMS_GEM_Analysis_Framework/issues

    and submit an issue that describes your problem (commands you executed, output, etc...).  You
    may need to provide any input files or a link to the fork of the repository for us to troubleshoot.

# Table of Contents
# ========================================================

    1. Contributors & License
    2. Installation Instructions
    3. Usage
        3.a. analyzeUniformity
            3.a.i           Helper Script - Run Mode: Grid
            3.a.ii          Helper Script - Run Mode: Rerun
            3.a.iii         Helper Script - Run Mode: Series
    4. Documentation
        4.a. Namespaces
        4.b. Class Map
            4.b.i.          Analyzers
                4.b.i.I     AnalyzeResponseUniformity
                4.b.i.II    AnalyzeResponseUniformityClusters
                4.b.i.III   AnalyzeResponseUniformityHits
            4.b.ii.         Selectors
                4.b.ii.I    Selector
                4.b.ii.II   SelectorClusters
                4.b.ii.III  SelectorHits
            4.b.iii.        DetectorMPGD
            4.b.iv.         InterfaceAnalysis
            4.b.v.          Loaders
                4.b.v.I     ParameterLoaderAmoreSRS
                4.b.v.II    ParameterLoaderAnalysis
                4.b.v.III   ParameterLoaderRun
            4.b.vi.         VisualizeUniformity
        4.c. Utilities
            4.c.i   Timing
            4.c.ii  Uniformity
        4.d. Types
            4.d.i   Timing
            4.d.ii  Uniformity
                4.d.ii.I    AnalysisSetupUniformity
                4.d.ii.II   Cluster
                4.d.ii.III  HistosPhysObj
                4.d.ii.IV   Hit
                4.d.ii.V    RunSetup
                4.d.ii.VI   SectorEta
                4.d.ii.VII  SectorPhi
                4.d.ii.VIII SectorSlice
                4.d.ii.IX   SelParam
                4.d.ii.X    SummaryStatistics
        4.e. Configuration Files
            4.e.i   amoreSRS Mapping Config File
            4.e.ii  Analysis Config File
                4.e.ii.I    HEADER PARAMETERS - ANALYSIS_INFO
                4.e.ii.II   HEADER PARAMETERS - TIMING_INFO
                4.e.ii.III  HEADER PARAMETERS - UNIFORMITY_INFO
                4.e.ii.IV   HEADER PARAMETERS - ADC_FIT_INFO
                4.e.ii.V    HEADER PARAMETERS - HISTO_INFO
                4.e.ii.VI   Example Config File
            4.e.iii Run Config File
                4.e.iii.I   HEADER PARAMETERS - RUN_INFO
                4.e.iii.II  HEADER PARAMETERS - RUN_LIST
                4.e.iii.III Configuration Options
                4.e.iii.IV  Example Config File - Mode: Series
                4.e.iii.V   Example Config File - Mode: Grid
                4.e.iii.VI  Example Config File - Mode: Re-Run
        4.f. Output ROOT File
                4.f.i.      "Segmented" Plots Stored in "Summary" folder
                4.f.ii.     "Dataset" Plots Stored in "Summary" folder
                4.f.iii.    1D Fit Summary Plots Stored in "Summary" folder
                4.f.iv.     2D Fit Trapezoidal Map Plots Stored in "Summary" folder
        4.g. Source Code Name Conventions
            4.g.i   STL Objects
            4.g.ii  ROOT Objects
    5. Known & Outstanding Issues

# 1. Contributors & License
# ========================================================

    Contributors: B. Dorney

    This package has been designed by B. Dorney with input from J. Merlin & S. Colafranceschi.
    The original selection & analysis algorithms are based off work done by J. Merlin.  This
    package makes use of several features from the CMS_GEM_TB_Timing repository (also by B. Dorney).
    Hopefully one day the CMS_GEM_TB_Timing repository will be fully integrated into this repository.

    This package makes use of the "C++ Mathematical Expression Library" designed by Arash Partow,
    available at (http://www.partow.net/programming/exprtk/index.html), and referred to as ExprTk.
    ExprTk is available under the "Common Public License" from the the url above, but has been
    included in this repository for convenience.

    The CMS_GEM_Analysis_Framework is licensed under the "GNU General Public License."

# 2. Installation Instructions
# ========================================================

    This is repository follows the guidelines of Vincent Driessen in
    "A successful Git branching model" available at:

    http://nvie.com/posts/a-successful-git-branching-model/

    The source code available in the origin/master branch always reflects a "production-ready"
    state.  The origin/develop branch reflects a state with the latest implemented changes
    for the next release.  This branch is not gauranteed to be stable.

    To checkout the repository:

        git clone https://github.com/bdorney/CMS_GEM_Analysis_Framework.git

        cd CMS_GEM_Analysis_Framework

        git pull origin master

    You will now have the latest version of the master branch.  To compile the repository:

        source scripts/setup_CMS_GEM.sh

        make -f Makefile.gpp clean

        make -f Makefile.gpp

    The repository is now compiled.  Additionally the base directory of the repository
    has been exported to the shell variable "$GEM_BASE".

    Please check https://github.com/bdorney/CMS_GEM_Analysis_Framework for the most-up-to-date
    release.  You migrate your master branch to the most-up-to-date branch via:

        git checkout -b <local_branch_name>

        git pull origin <remote_branch_name>

        make -f Makefile.gpp clean

        make -f Makefile.gpp

    The local_branch_name is the name of the branch on your local machine.The
    remote_branch_name is the name of the branch you are checking out from the remote repository.
    It is a good practice to make local_branch_name = remote_branch_name.  You can see the list
    of available branches from command line via:

        git fetch

        git branch -a

    The branch you are currently on will have the "*" character next to it.

    NOTE: a make file for clang has been included "Makefile.clang" for MAC OS users.  However
    presently there is no support for any installation/runtime errors on a MAC OS environemnt.
    It is strongly urged that you use the Linux computing environment mentioned above (since it
    is so readily available to us).

# 3. Usage
# ========================================================

    The following gives the usage case for each of the executables produced during the installation.
    It is assumed that you have successfully performed the steps listed in Section 2.

    # 3.a. analyzeUniformity
    # --------------------------------------------------------

    For each new shell navigate to the base directory of the repository and setup the environment via:

        source scripts/setup_CMS_GEM.sh

    The usage for the analyzeUniformity executable is:

        For help menu:  ./analyzeUniformity -h
        For executing:  ./analyzeUniformity <PFN of Run Config File> <Verbose Mode true/false>

    Here the physical file name (PFN) represents the full path+filename to the file in question.
    The configuration files, including the run config file) are described in Section 4.e.  The
    executable can analyze files produced either by amoreSRS/amoreSRS_ZS or by the
    CMS_GEM_Analysis_Framework.  For a full explanation of the available configurations please
    consult 4.e.

    The contents and layout of the output root file are described in Section 4.f.

    Three example config files: 1) mapping config file, 2) analysis config file, and 3) run config
    file have been provided in the default repository.  A usage example is given as:

        ./analyzeUniformity config/configRun.cfg true

            # 3.a.i Helper Script - Run Mode: Grid
            # --------------------------------------------------------

            The script:

                scripts/runMode_Grid.sh

            is for running the framework the lxplus batch submission system using the scheduler bsub.
            This script will setup the run config file and launch one job for each input file in the
            data file directory below.  The expected synatx is:

                source scripts/runMode_Grid.sh <Data File Directory> <Config File - Analysis> <Config File - Mapping> <Queue Names>

            Where: "Data File Directory" is the physical file path (PFP) where the input data files to
            be analyzed are located, "Config File - Analysis" is the PFN of the input analysis config
            file, "Config File - Mapping" is the input mapping config file, and "Queue Names" are the
            requested submission queue on the lxplus batch submmission system.  The available queues
            on lxplus are {8nm, 1nh, 8nh, 1nd} for 8 natural minutes, 1 natural hour, 8 natural hours,
            and 1 natural day, respectively.

            Additionally for each job a run config file (described in Section 4.e.iii.V), named
            config/configRun_RunNoX.cfg where X is the job number, will be created.  One script per job, named
            scripts/submitFrameworkJob_RunNoX.sh, will created; this script will be what the job executes when
            it runs.  Three directories will also be created, if they do not already exist, called $GEM_BASE/stderr,
            $GEM_BASE/stdlog, and $GEM_BASE/stdout.  Each of these directories will respectively store the
            stderr (stderr/frameworkErr_RunNoX.txt), stdlog (stdlog/frameworkLog_RunNoX.txt), and stdout
            (stdout/frameworkOut_RunNoX.txt) files produced for each job.  The stderr file for a job should be
            checked if a job fails to produce an output TFile.  The stdlog file for a job shows any output to
            terminal that would be created if the executable was run locally.  The stdout file shows a summary
            of the job prepared by the scheduler showing time taken, memory usage, and stderr file for the job.

            After scripts/runMode_Grid.sh has submitted all your jobs it will dump some useful information for
            how to check running jobs and kill running jobs if necessary.  The scripts/runMode_Grid.sh will also
            show how to add all output TFiles together from command line using the "hadd" command.  After you have
            added all TFiles together you should run the framework over this summary TFile in re-run mode to
            perform the fitting and uniformity analysis.

            After you have had a chance to investigate the created run config files, submission scripts, stderr,
            stdlog, and stdout files you can safely remove them by calling:

                source scripts/cleanGridFiles.sh

            Eample:

                source scripts/runMode_Grid.sh $DATA_QC5/GE11-VII-L-CERN-0001 config/configAnalysis.cfg config/Mapping_GE11-VII-L.cfg 1nh
                cd $DATA_QC5/GE11-VII-L-CERN-0001
                hadd summaryFile_Ana.root *Ana.root
                cd $GEM_BASE
                source scripts/cleanGridFiles.sh
                source scripts/runMode_Rerun.sh GE11-VII-L-CERN-0001 $DATA_QC5/GE11-VII-L-CERN-0001 config/configAnalysis.cfg config/Mapping_GE11-VII-L.cfg

            NOTE: Modications to config/configRun_Template_Grid.cfg may lead to undefined behavior or failures;
            it is recommended to not modify the template config file.

            # 3.a.ii Helper Script - Run Mode: Rerun
            # --------------------------------------------------------

            The script:

                scripts/runMode_Rerun.sh

            is for running the framework over a previously produced framework output TFile. This script will
            setup the run config file to re-run over each input file found in the data file directory below.
            One output TFile will be produced for each input file.  The expected synatx is:

                source scripts/runMode_Rerun.sh <Detector Name> <Data File Directory> <Config File - Analysis> <Config File - Mapping>

            Where: "Detector Name" is the detector serial number; and "Data File Directory," "Config File - Analysis,"
            and "Config File - Mapping" are as described in Section 3.a.i.

            After calling this script it is recommended to cross-check the created config/configRun.cfg file
            before executing the framework.  This will let you ensure the correct set of input files will be
            re-analyzed.

            Example:

                source scripts/runMode_Rerun.sh GE11-VII-L-CERN-0001 $DATA_QC5/GE11-VII-L-CERN-0001 config/configAnalysis.cfg config/Mapping_GE11-VII-L.cfg
                ./analyzeUniformity config/configRun.cfg true

            NOTE: Modications to config/configRun_Template_Rerun.cfg may lead to undefined behavior or failures;
            it is recommended to not modify the template config file.

            # 3.a.iii Helper Script - Run Mode: Series
            # --------------------------------------------------------

            The script:

                scripts/configRun_Template_Series.cfg

            is for running the framework over a set of input files in series, i.e. one after the other, and
            creating a single output TFile.  The expected syntax is:

                            source runMode_Series.sh <Detector Name> <Data File Directory> <Config File - Analysis> <Config File - Mapping> <Output Data Filename>

            Where: "Detector Name," "Data File Directory," "Config File - Analysis," and "Config File - Mapping"
            are as described in Sections 3.a.i and 3.a.ii; and "Output Data Filename" is the desired name of
            the output TFile to be created.

            After calling this script it is recommended to cross-check the created config/configRun.cfg file
            before executing the framework.  This will let you ensure the correct set of input files will be analyzed.

            Example:

                source runMode_Series.sh GE11-VII-L-CERN-0001 $DATA_QC5/GE11-VII-L-CERN-0001 config/configAnalysis.cfg config/Mapping_GE11-VII-L.cfg GE11-VII-L-CERN-0001_FrameworkAna.root
                ./analyzeUniformity config/configRun.cfg true

            NOTE: Modications to config/configRun_Template_Series.cfg may lead to undefined behavior or failures;
            it is recommended to not modify the template config file.

# 4. Documentation
# ========================================================

    This section describes the contents of the repository, the expected inputs, and the produced outputs.
    Developers should have a firm grasp of this entire section; users need only be concerned with sections
    4.e and 4.f.

    # 4.a. Namespaces
    # --------------------------------------------------------

    This repository at presently makes use of two namespaces: Timing, Uniformity.  The Timing namespace
    includes several operators, types, and utility functions that were developed in CMS_GEM_TB_Timing;
    the contents of the Timing namespace offer substantial utility and "quality of life features."

    The Uniformity namespace contains the majority of source code for analyzing response uniformity
    measurements performed for GE1/1 detectors; and hopefully GE2/1 & ME0 detectors in the future.

    # 4.b. Class Map
    # --------------------------------------------------------

    Inheritance relations:

        AnalyzeResponseUniformityClusters, AnalyzeResponseUniformityHits, and VisualizeUniformity all
        inherit from AnalyzeResponseUniformity.

        SelectorHit and SelectorClusters all inherit from Selector.

    Friendship relations:

        AnalyzeResponseUniformity, AnalyzeResponseUniformityClusters, AnalyzeResponseUniformityHits,
        and ParameterLoaderAmoreSRS are all friend classes to DetectorMPGD.

    Interactions:

        Classes ParameterLoaderAmoreSRS, those inheriting from Selector, and those inheriting from
        AnalyzeResponseUniformity all act on an object of DetectorMPGD.

        The ParameterLoaderAnalysis class interacts with objects who inherit from Selector and
        AnalyzResponseUniformity classes.

        InterfaceAnalysis -> interface between main() and the framework; runs the analysis for loaded case.
        ParameterLoaderAmoreSRS -> creates a DetectorMPGD object
        ParameterLoaderAnalysis -> sets up the user specified analysis; this info is passed separately to Selector & AnalyzeResponseUniformity classes (and their inherited classes).
        ParameterLoaderRun -> sets up the run configuration, the files to be analyzed, and what analysis stages (e.g. hits, clusters, fitting, etc...) to be exectued.
        SelectorCluster -> Assigned an input DetectorMPGD object the opens an input root file and performs the cluster selection; selected clusters are stored based on their location in the DetectorMPGD object.
        SelectorHit -> As SelectorCluster but for hits (e.g. single strips).
        AnalyzeResponseUniformityCluster -> Acts on a DetectorMPGD object that has stored clusters and performs the user requested analysis
        AnalyzeResponseUniformityHit -> As AnalyzeResponseUniformityCluster but for hits.
        VisualizeUniformity -> Takes the raw plots produced by AnalyzeResponseUniformity and presents them in a user friendly manner.

        # 4.b.i. Analyzers
        # --------------------------------------------------------

        Coming "soon"

            # 4.b.i.I AnalyzeResponseUniformity
            # --------------------------------------------------------

            Coming "soon"

            # 4.b.i.II AnalyzeResponseUniformityClusters
            # --------------------------------------------------------

            Coming "soon"

            # 4.b.i.III AnalyzeResponseUniformityHits
            # --------------------------------------------------------

            Coming "soon"

        # 4.b.ii. Selectors
        # --------------------------------------------------------

        These classes are for performing the cluster/event/hit selection specified by the user.

            # 4.b.ii.I Selector
            # --------------------------------------------------------

            Coming "soon"

            # 4.b.ii.II SelectorCluster
            # --------------------------------------------------------

            Coming "soon"

            # 4.b.ii.III SelectorHit
            # --------------------------------------------------------

            Coming "soon"

        # 4.b.iii. DetectorMPGD
        # --------------------------------------------------------

        Coming "soon"

        # 4.b.iv. InterfaceAnalysis
        # --------------------------------------------------------

        Coming "soon"

        # 4.b.v. Loaders
        # --------------------------------------------------------

        These classes are loading user specified parameters at runtime.

            # 4.b.v.I ParameterLoaderAmoreSRS
            # --------------------------------------------------------

            Coming "soon"

            # 4.b.v.II ParameterLoaderAnalysis
            # --------------------------------------------------------

            Coming "soon"

            # 4.b.v.III ParameterLoaderRun
            # --------------------------------------------------------

            Coming "soon"

        # 4.b.vi. VisualizeUniformity
        # --------------------------------------------------------

        Coming "soon"

    # 4.c. Utilities
    # --------------------------------------------------------

    For utility functions defined in the Timing namespace see
    $GEM_BASE/include/TimingUtilityFunctions.h and it's implementation in
    $GEM_BASE/src/TimingUtilityFunctions.cpp.

    For utility functions defined in the Uniformity namespace see
    $GEM_BASE/include/UniformityUtilityFunctions.h and it's implementation in
    $GEM_BASE/src/UniformityUtilityFunctions.cpp.

        # 4.c.i. Timing
        # --------------------------------------------------------

        Coming "soon"


        # 4.c.ii. Uniformity
        # --------------------------------------------------------

        Coming "soon"

    # 4.d Types
    # --------------------------------------------------------

    For types defined in the Timing namespace see
    $GEM_BASE/include/TimingUtilityTypes.h and it's implementation in
    $GEM_BASE/src/TimingUtilityTypes.cpp.

    For utility functions defined in the Uniformity namespace see
    $GEM_BASE/include/UniformityUtilityTypes.h and it's implementation in
    $GEM_BASE/src/UniformityUtilityTypes.cpp.

        # 4.d.i. Timing
        # --------------------------------------------------------

        More Coming "soon"

        The Timing::HistoSetup struct is used for storing user input defined in the Analysis Config file.
        Objects of this struct store values to be set to data members of TH1 and TF1 objects.

        An instance of the Timing::HistoSetup struct is used by private methods of the
        Uniformity::AnalyzeResponseUniformity class for producing & manipulating TH1 & TF1 objects.

        Data members of Timing::HistoSetup struct relevant to the ROOT::TH1 class are:

            HistoSetup::fHisto_xLower       //lower x range of a TH1 object
            HistoSetup::fHisto_xUpper       //upper x range of a TH1 object

            HistoSetup::iHisto_nBins        //number of bins a TH1 object will have

            HistoSetup::strHisto_Name       //TName of a TH1 object
            HistoSetup::strHisto_Title_X    //X-axis title of a TH1 object
            HistoSetup::strHisto_Title_Y    //Y-axis title of a TH1 object

        Data members of Timing::HistoSetup struct relevant to the ROOT::TF1 class are:

            HistoSetup::strFit_Formula              //TFormula of a TF1 object
            HistoSetup::strFit_Name                 //TName of a TF1 object
            HistoSetup::strFit_Option               //Option to be used when fitting some TObject with a TF1
            HistoSetup::vec_strFit_ParamMeaning     //Vector storing meaning of each fit parameter
            HistoSetup::vec_strFit_ParamIGuess      //Vector storing initial guess of each parameter (stof conversion occurs)
            HistoSetup::vec_strFit_ParamLimit_Min   //Vector storing lower bound of each parameter (stof conversion occurs)
            HistoSetup::vec_strFit_ParamLimit_Max   //Vector storing upper bound of each parameter (stof conversion occurs)
            HistoSetup::vec_strFit_Range            //Vector storing range of TF1 object

        In the case of the vector members above, stof conversion occurs via the utility
        function Timing::stofSafe.  Unfortunately scientific notation is not presently supported.
        However this allows the user to enter complex expressions beyond simple numeric input.

        The following members of Timing::HistoSetup are not used presently (legacy from CMS_GEM_TB_Timing,
        kept in case the two repositories are ever merged):

            HistoSetup::bFit
            HistoSetup::bFit_AutoRanging
            HistoSetup::bIsTrig
            HistoSetup::iTDC_Chan;

        # 4.d.ii. Uniformity
        # --------------------------------------------------------

        The list of structs defined in namespace Uniformity is as follows:

            AnalysisSetupUniformity
            Cluster
            HistoPhysObj
            Hit
            RunSetup
            SectorEta
            SectorPhi
            SectorSlice
            SelParam
            SummaryStatistics

        Each of these items are described in detail below.

        Several of the above structs have both copy constructors and overloaded assignment operators which perform
        a deep copy of all the objects stored in the struct.  For those unfamiliar with the term "deep copy" please
        consult:

            http://stackoverflow.com/questions/184710/what-is-the-difference-between-a-deep-copy-and-a-shallow-copy

        or perform a google search of "deep copy C++" for further explanation.

            # 4.d.ii.I AnalysisSetupUniformity
            # --------------------------------------------------------

            The AnalysisSetupUniformity struct stores user input defined in the Analysis Config file.  This
            struct has one instance of a HistoSetup struct for each cluster physical obserable (e.g. ADC,
            position, etc...).  Additionally this struct has one instance of a SelParam struct; which
            stores user input defined in the Analysis Config file for cluster selection.

            Data members of Uniformity::AnalysisSetupUniformity are:

                AnalysisSetupUniformity::iEvt_First             //First event of TTree, produced by amoreSRS, to process.
                AnalysisSetupUniformity::iEvt_Total             //Total events of a TTree, produced by amoreSRS, to process.
                AnalysisSetupUniformity::iUniformityGranularity //The level of granularity the analysis will be carried out on  all iPhi sectors of a DetectorMPGD object (e.g. a value of 128 means at the strip level, a value of 2 means two groups of 64 strips).
                AnalysisSetupUniformity::fUniformityTolerance   //The requested tolerance on the gain uniformity

                AnalysisSetupUniformity::histoSetup_clustADC    //HistoSetup obejct for specifying cluster ADC TH1 & TF1 objects
                AnalysisSetupUniformity::histoSetup_clustMulti  //HistoSetup obejct for specifying cluster multiplicity TH1 objects
                AnalysisSetupUniformity::histoSetup_clustPos    //HistoSetup obejct for specifying cluster position TH1 objects
                AnalysisSetupUniformity::histoSetup_clustSize   //HistoSetup obejct for specifying cluster size TH1 objects
                AnalysisSetupUniformity::histoSetup_clustTime   //HistoSetup obejct for specifying cluster time bin TH1 objects

                AnalysisSetupUniformity::histoSetup_hitADC      //HistoSetup object for specifying hit ADC TH1 objects
                AnalysisSetupUniformity::histoSetup_hitPos      //HistoSetup object for specifying hit position TH1 objects (in strip No.)
                AnalysisSetupUniformity::histoSetup_hitTime     //HistoSetup object for specifying hit time bin TH1 objects

                AnalysisSetupUniformity::selClust               //SelParam object specifying cluster selection cuts
                AnalysisSetupUniformity::selHit                 //SelParam object specifying hit selection cuts

            # 4.d.ii.II Cluster
            # --------------------------------------------------------

            The Uniformity::Cluster struct stores information relating to one reconstructed cluster
            stored in the TCluster TTree created by amoreSRS.  Data members of Uniformity::Cluster are:

                Cluster::iPos_Y     //Distance in mm from wide base of trapezoid to cluster (e.g. vertical midpoint of iEta sector)
                Cluster::fPos_X     //Horizontal position within iEta sector (used to assign to correct iPhi sector)
                Cluster::fADC       //ADC value of cluster
                Cluster::iSize      //Number of strips in cluster
                Cluster::iTimeBin   //Time bin, e.g. latency value, of the cluster

                Cluster::map_hits   //map of Uniformity::Hit objects used to reconstruct this cluster.  Note presently just a placeholder.  Empty for all clusters presently.

            Note data types of Uniformity::Cluster (e.g. int, float, etc...) should closely match what amoreSRS stores
            in TCluster TTree; e.g. ADC is intrinscially integer physically, but it is defined as a float in amoreSRS.

            # 4.d.ii.III HistosPhysObj
            # --------------------------------------------------------

            The Uniformity::HistosPhysObj struct is used as a container for ROOT histograms (i.e. TH1, TH2, etc...).
            These histograms are tracked at varying levels of the DetectorMPGD geometry (e.g. per SectorEta, per
            SectorPhi, etc...).  Each data member of Uniformity::HistosPhysObj is a std::shared_ptr of a ROOT object,
            they are given specifically as:

                HistosPhysObj::hADC             //ADC Spectrum for some physics object (e.g. clusters, hits, etc...)
                HistosPhysObj::hMulti           //Multiplicity "                                                    "
                HistosPhysObj::hPos             //Position     "                                                    "
                HistosPhysObj::hSize            //Size         "                                                    "
                HistosPhysObj::hTime            //Time bin (e.g. latency) "                                         "
                HistosPhysObj::hADC_v_EvtNum    //PENDING IMPLEMENTATION ADC vs event number "                                             "
                HistosPhysObj::hADC_v_Pos       //ADC vs Position "                                                 "
                HistosPhysObj::hADC_v_Size      //ADC vs Size "                                                     "
                HistosPhysObj::hADC_v_Time      //ADC vs Latency "                                                  "

                HistosPhysObj::hADCMax_v_ADCInt //Max ADC (from all latency bins) of an object vs Integral of object's ADC (sum of all latency bins)

            For clusters hPos is the position along the detector trapezoid in mm with the detector axis being 0 mm in
            the iPhi=2 sector, negative (positive) position values occur in iPhi = 1 (3).  For hits the position is
            the strip number along the detector from 0 to 383 increasing with increasing iPhi.

            There is also one copy constructor and one overloaded assignment operator.  These items perform a
            deep copy of the std::shared_ptr objects above.

            # 4.d.ii.IV Hit
            # --------------------------------------------------------

            The Uniformity::Hit struct stores information relating to one reconstructed hit stored in the THit TTree
            created by amoreSRS.  Data members of Uniformity::Hit are:

                Hit::iPos_Y         //Distance in mm from wide base of trapezoid to hit (e.g. vertical midpoint of iEta sector)
                Hit::iStripNum      //Strip number of the hit, this ranges from 0 to 383?
                Hit::iTimeBin       //Time bin, e.g. latency value, of the hit
                Hit::sADCIntegral   //Integral of ADC values from all time bins
                Hit::vec_sADC       //Vector of ADC values, each element of the vector represents ADC value at that time bin; e.g. vec_sADC[Hit::iTimeBin] gives the ADV value at the defined time bin

            Again, data types of Uniformity::Hit should match what amoreSRS stores in the THit TTree.

            # 4.d.ii.V RunSetup
            # --------------------------------------------------------

            The Uniformity::RunSetup struct stores user input defined in the RUN_INFO header of the Run Config
            File.  It does not store the list of input files.  This struct is responsible for setting the analysis
            configuration and is used by InterfaceAnalysis class for identifying what stages of the analysis
            should be performed.

            Data members of Uniformity::RunSetup are:

                RunSetup::bAnaStep_Clusters     //True: perform the cluster analysis; false: do not.
                RunSetup::bAnaStep_Fitting      //True: fit stored distributions; false: do not.  Note bAnaStep_Clusters (bAnaStep_Hits) must also be true for cluster (hit) distributions to be fitted.
                RunSetup::bAnaStep_Hits         //True: perform the hit analysis; false: do not.
                RunSetup::bAnaStep_Reco         //True: reconstruct clusters from hits; false: take clusters from input amoreSRS TTree.  Right now just a placeholder value.  Does nothing presently.
                RunSetup::bAnaStep_Visualize    //True: make summary TCanvas objects after analyzing all input files; false: do not.

                RunSetup::bInputFromFrmwrk      //True (false): input files are produced by the CMS_GEM_Analysis_Framework (amoreSRS)

                RunSetup::bLoadSuccess          //True (false): the input parameters were (not) loaded successfully from the Run Config File. Defaults to false.  If after attempting to load these parameters from the Run Config File this is still false the analysis routine exits.

                RunSetup::bMultiOutput          //True: one output file is made which represents the sum of all input files; false: one output file is made for each input file.  Note when bInputFromFrmwrk is true bMultiOutput must also be true.

                RunSetup::bVisPlots_PhiLines    //True: draw lines denoting iPhi sectors in plots spanning iEta sectors; false: do not.
                RunSetup::bVisPlots_AutoSaving  //True: automatically save TCanvas objects stored in the Summary folder of the output TFile as *.png and *.pdf files; false: do not.

                RunSetup::strFile_Config_Ana    //PFN of input analysis config file
                RunSetup::strFile_Config_Map    //PFN of input mapping config file

                RunSetup::strFile_Output_Name   //PFN of output TFile to be created when bMultiOutput is false
                RunSetup::strFile_Output_Option //Option for TFile, e.g. CREATE, RECRATE, UPDATE, etc...

                RunSetup::strDetName            //Stores a string acting as the unique detector serial number.  Resolves ambiguity in TObject TName data members when opening multiple output TFiles.

            # 4.d.ii.VI SectorEta
            # --------------------------------------------------------

            The Uniformity::SectorEta struct represents one iEta row of a detector. Each instance of a
            Uniformity::SectorEta will store nbConnect objects of a Uniformity::SectorPhi struct where
            nbConnect is a field found in the amoreSRS mapping file defining the number of readout conncetors
            per iEta row. Each object of a Uniformity::SectorPhi struct will store
            Uniformity::AnalysisSetupUniformity::iUniformityGranularity number of Uniformity::SectorSlice
            struct objects.  An object of a Uniformity::DetectorMPGD class will store a number of
            objects of Uniformity::SectorEta as defined in the amoreSRS mapping file (e.g. number of "DET" rows).

            The data members of the Uniformity::SectorEta struct are:

                SectorEta::fPos_Y                       //Vertical Midpoint, in mm, of iEta row from wide base of trapezoid
                SectorEta::fWidth                       //Width of iEta sector, in mm, at SectorEta::fPos_Y;
                SectorEta::map_sectorsPhi               //Container storing three instances of SectorPhi objects
                SectorEta::mset_fClustADC_Fit_PkPos     //Container storing peak position from Clust ADC Fit
                SectorEta::mset_fClustADC_Spec_PkPos    //Container storing peak position from TSpectrum::Search() & TSpectrum::GetPositionX()
                SectorEta::gEta_ClustADC_Fit_NormChi2   //std::shared_ptr of a TGraphErrors storing NormChi2 of fits from all SectorSlice::hSlice_ClustADC
                SectorEta::gEta_ClustADC_Fit_PkPos      //std::shared_ptr of a TGraphErrors storing ADC spec peak position from fits of all SectorSlice::hSlice_ClustADC
                SectorEta::gEta_ClustADC_Fit_PkRes      //std::shared_ptr of a TGraphErrors storing ADC spec peak resolution.  Resolution is taken as (FWHM / Mean).  The FWHM and mean are taken from the fit results from fits of all SectorSlice::hSlice_ClustADC
                SectorEta::gEta_ClustADC_Fit_Failures   //As SectorEta::gEta_ClustADC_Fit_PkPos but for when the minimizer did not succeed in finding a minima
                SectorEta::gEta_ClustADC_Spec_NumPks    //std::shared_ptr of a TGraphErrors storing number of peaks found in the SectorSlice::hSlice_ClustADC histogram; based on TSpectrum::Search() and TSpectrum::GetNPeaks()
                SectorEta::gEta_ClustADC_Spec_PkPos     //As SectorEta::gEta_ClustADC_Fit_PkPos but from TSpectrum::Search() and TSpectrum::GetPositionX() instead of fitting
                SectorEta::clustHistos                  //An instance of the Uniformity::HistosPhysObj struct for Clusters; at present time all members of the struct are used except hMulti
                SectorEta::hitHistos                    //An instance of the Uniformity::HistosPhysObj struct for Hits; at present time only hPos and hTime are used
                SectorEta::statClustADC_Fit_PkPos       //An instance of the Uniformity::SummaryStatistics struct for results of the cluster ADC peak position fitting
                SectorEta::statClustADC_Spec_PkPos      //As SectorEta::statClustADC_Fit_PkPos but from TSpectrum::Search() & TSpectrum::GetPositionX()

            There is also one copy constructor and one overloaded assignment operator.  These items perform a
            deep copy of the std::shared_ptr objects above.

            4.d.ii.VII SectorPhi
            # --------------------------------------------------------

            The data members of the Uniformity::SectorPhi struct are:

                SectorPhi::fPos_Xlow                //X lower boundary of iPhi sector, in mm, at SectorEta::fPos_Y;
                SectorPhi::fPos_Xhigh               //X upper boundary of iPhi sector, in mm, at SectorEta::fPos_Y;
                SectorPhi::fWidth                   //Width of iPhi sector, in mm, at SectorEta::fPos_Y;
                SectorPhi::iStripNum_Min            //lower bound of strip number for this iPhi sector, e.g. 0, 128, 256
                SectorPhi::iStripNum_Max            //upper bound of strip number for this iPhi sector, e.g. 127, 255, 383
                SectorPhi::map_slices               //Container storing Uniformity::AnalysisSetupUniformity::iUniformityGranularity number of Uniformity::SectorSlice objects
                SectorPhi::vec_clusters             //vector of stored Uniformity::Cluster located in this SectorPhi (iPhi value)
                SectorPhi::vec_hits                 //vector of stored Uniformity::Hit located in this SectorPhi (iPhi value)
                SectorPhi::clustHistos              //As SectorEta::clustHistos but only for this SectorPhi (iPhi value)
                SectorPhi::hitHistos                //As SectorEta::hitHistos but only for this SectorPhi (iPhi value)

            There is also one copy constructor and one overloaded assignment operator.  These items perform a
            deep copy of the std::shared_ptr objects above.

            4.d.ii.VIII SectorSlice
            # --------------------------------------------------------

            The data members of the Uniformity::SectorSlice struct are:

                SectorSlice::fPos_Center        //Location of the center of the slice, in mm, within the SectorPhi (iPhi value)
                SectorSlice::fWidth             //Width of the slice in mm
                SectorSlice::fitSlice_ClustADC  //std::shared_ptr of a TF1; used to fit SectorSlice::hSlice_ClustADC
                SectorSlice::hSlice_ClustADC    //As SectorEta::hEta_ClustADC but only for this SectorSlice

            There is also one copy constructor and one overloaded assignment operator.  These items perform a
            deep copy of the std::shared_ptr objects above.

            4.d.ii.IX SelParam
            # --------------------------------------------------------

            Data members of Uniformity::SelParam are:

                SelParam::iCut_ADCNoise     //Hit or Cluster rejected if ADC LESS than value
                SelParam::iCut_ADCSat       //Hit rejected if ADC GREATER than value
                SelParam::iCut_MultiMin     //EVENT rejected if cluster multiplicity LESS than or equal to value
                SelParam::iCut_MultiMax     //EVENT rejected if cluster multiplicity GREATER than or equal to value
                SelParam::iCut_SizeMin      //Cluster rejected if size LESS than value
                SelParam::iCut_SizeMax      //Cluster rejected if size GREATER than value
                SelParam::iCut_TimeMin      //Hit or Cluster rejected if time bin LESS than value
                SelParam::iCut_TimeMax      //Hit or Cluster rejected if time bin GREATER than value

            4.d.ii.X SummaryStatistics
            # --------------------------------------------------------

            The Uniformity::SummaryStatistics is a container for storing statistical parameters of a dataset
            (e.g. fit results from all strips).  Data members of Uniformity::SummaryStatistics are:

                SummaryStatistics::fIQR             //Interquantile range of dataset; IQR = Q3 - Q1; intrinsically positive
                SummaryStatistics::fMax             //Max value of dataset
                SummaryStatistics::fMean            //Mean value of dataset
                SummaryStatistics::fMin             //Min value of dataset
                SummaryStatistics::fQ1              //First quantile (Q1) of dataset
                SummaryStatistics::fQ2              //Second quantile (Q2) of dataset
                SummaryStatistics::fQ3              //Third quantile (Q3) of dataset
                SummaryStatistics::fStdDev          //Standard deviation of dataset
                SummaryStatistics::mset_fOutliers   //Container storing outliers of a dataset; outlier definition is defined per Analyzer
                SummaryStatistics::hDist            //TH1F object of which stores the distribution of the dataset

            There is also one copy constructor and one overloaded assignment operator.  These items perform a
            deep copy of the std::shared_ptr objects above.

    # 4.e. Configuration Files
    # --------------------------------------------------------

    Two configuration files are require to run the analyzeUniformity exectuable.  The first is the amoreSRS
    mapping file.  The second is the Analysis Config file.  Examples of both files have been included in
    the $GEM_BASE/config directory

        # 4.e.i. amoreSRS Mapping Config File
        # --------------------------------------------------------

        The amoreSRS mapping config file is the file used by amoreSRS when analyzing data collected with the SRS
        system.  Right now only the part of the file specifying the detectors (e.g. lines starting with
        "DET") are used:

            # CMSGEM
            #################################################################################################
            #   ReadoutType  DetType    DetName    Sector     SectPos   SectSize   nbConnect  orient
            #################################################################################################
            DET,   CMSGEM,    CMSGEM,    CMS,    CMSSECTOR1,    139,         401,       3,       1
            ...     ...         ...      ...        ...         ...          ...        ...     ...
            DET,   CMSGEM,    CMSGEM,    CMS,    CMSSECTOR8,    1169,        223,       3,       1

        For each "DET" line defined in this file an entry in the DetectorMPGD::map_sectorsEta map will be
        stored.  In this way the amoreSRS mapping config file specifies the geometry of the DetectorMPGD object.

        # 4.e.ii. Analysis Config File
        # --------------------------------------------------------

        The analysis config file expects a certain "nested-header" style. The format should look something like:

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

        For each of the higher level header sections (i.e. "[BEGIN_ANALYSIS_INFO]" and "[BEGIN_UNIFORMITY_INFO]")
        the header parameters should *always* be placed before the declaration of lower headers.  Placing top
        level header parameters after the declaration of lower level headers could lead to undefined behavior
        or at worst crashes.

        Parameters are expected to be entered in the following format:

            field_name = 'value';

        The field_name should be on the left followed by an equals sign "=" then the value should be enclosed
        in single quote marks "'".  A semicolon ";" ends the statement.  Tabs "\t" and spaces outside of the
        single quotes will be ignored, but will be preserved inside the single quotes.  Text after the ";" will
        also be ignored.  The template at the end of this subsection showns an example.

        The Uniformity::ParameterLoaderAnalysis class understands the "#" character to indicate a comment; so
        it is possible to comment out lines in the Analysis Config file you create for ease of use.

        Please note that for all field_names that scientific notation is, unfortunately, not yet supported and
        WILL lead to crashes (i.e. enter 1000 instead of 1e3).

        # 4.e.ii.I HEADER PARAMETERS - ANALYSIS_INFO
        # --------------------------------------------------------

            The following parameters are supported:
            #		<FIELD>			<DATA TYPE, DESCRIPTION>

        None for now, placeholder

        # 4.e.ii.II HEADER PARAMETERS - TIMING_INFO
        # --------------------------------------------------------

            The following parameters are supported:
            #		<FIELD>			<DATA TYPE, DESCRIPTION>

        None for now, placeholder

        # 4.e.ii.III HEADER PARAMETERS - UNIFORMITY_INFO
        # --------------------------------------------------------

        The table below describes the allowed input fields and their data types.

            The following parameters are supported:
            #		<FIELD>             <DATA TYPE, DESCRIPTION>

                Cut_ADC_Min             integer, clusters with ADC values less than this value are rejected.

                Cut_ClusterMulti_Min    integer, events with clusters multiplicity less than or equal to this
                                        value are rejected.

                Cut_ClusterMulti_Max    integer, events with clusters multiplicity greater than or equal to this
                                        value are rejected.

                Cut_ClusterSize_Min     integer, clusters with sizes less than this value are rejected.

                Cut_ClusterSize_Max     integer, clusters with sizes greater than this value are rejected.

                Cut_ClusterTime_Min     integer, clusters with time bins less than this value are rejected.

                Cut_ClusterTime_Max     integer, clusters with time bins greater than this value are rejected.

                Cut_HitAdc_Min          integer, hits with ADC value less than this value are rejected.

                Cut_HitAdc_Max          integer, hits with ADV value greater than this value are rejected.

                Cut_HitMulti_Min        integer, events with hit multiplicity less than or equal to this
                                        value are rejected.

                Cut_HitMulti_Max        integer, events with hit multiplicity greater than or equal to this
                                        value are rejected.

                Cut_HitTime_Min         integer, hits with time bins less than this value are rejected.

                Cut_HitTime_Max         integer, hits with time bins greater than this value are rejected.

                Event_First             integer, the first event, of the TTree found in the input root file
                                        (created by amoreSRS with ROOTDATATYPE = CLUSTERS_ONLY), to start
                                        processing the cluster selection at.

                Event_Total             integer, total number of events to process from the Event_First of the
                                        TTree found in the input root file (created by amoreSRS with
                                        ROOTDATATYPE = CLUSTERS_ONLY) A value of -1 sets indicates all events
                                        from the first event will be processed.

                Uniformity_Granularity  integer, numer of slices, or partitions, to split one iPhi sector into
                                        for the response uniformity measurement.

                Uniformity_Tolerance    float, checks if fabs( (Det_Resp_Max - Det_Resp_Min) / Det_Resp_Max )
                                        is less than the input value.  If it is the response uniformity is
                                        considered to be within the tolerance.  Expected to be from 0.0 to 1.0

        # 4.e.ii.IV HEADER PARAMETERS - ADC_FIT_INFO
        # --------------------------------------------------------

        A set of keywords = {"AMPLITUDE","MEAN","PEAK","SIGMA"} is presently supported which
        allows the user to configure complex expressions for the initial guess of fit parameters,
        their limits, and the fit range.  In the future we hope to add additional keywords (e.g.
        "HWHM" and "FWHM").  The table below describes the keywords:

            The following keywords are supported:
            #       <KEYWORD>       <DESCRIPTION>

                AMPLITUDE           The fit parameter is set based on the value of the TH1 bin with the largest
                                    content (e.g. TH1::GetBinContent(TH1::GetMaximumBin() ) ).

                MEAN                The fit parameter is set based on the mean of the distribution stored in
                                    the TH1 object (e.g. TH1::GetMean() ).

                PEAK                An instance of the ROOT::TSpectrum class is created and searches the TH1
                                    object that will be fitted for all peaks.  The TSpectrum parameters are
                                    hard coded and "should" ensure only one peak is found (open issue).  The
                                    x-position of the first peak found in the TH1 distribution is set to the
                                    fit parameter (e.g. TSpectrum::Search() and TSpectrum::GetPositionX()
                                    are used).

                SIGMA               The fit parameter is set based on the SIGMA of the distribution stored in
                                    the TH1 object (e.g. TH1::GetRMS() ).


        The table below describes the allowed input fields and their data types:

            The following parameters are supported:
            #		<FIELD>             <DATA TYPE, DESCRIPTION>

                Fit_Formula             string, TFormula to be given to a ROOT::TF1 object.  Note the syntax
                                        and full complexity of inputs expected/available in ROOT works!
                                        e.g. "[0]*x^2+[1]" or "gaus(0)+pol2(3)" or "[1]*TMath::Erf(x) + [2]"
                                        are all supported.

                Fit_Option              string, the fit option to be used for fitting the ADC spectrums made
                                        from each slice.

                Fit_Param_IGuess        string, comma separated list of the initial values for the parameters
                                        defined in the Fit_Formula field.  Note the order in which the parameters
                                        are given in the Fit_Formula field should match the order listed here.
                                        Explicitly for the Fit_Formula '[0]*x^2+[1]*x+[2]' the initial guess
                                        of '4,5,6' would mean [0] = 4, [1] = 5, and [2] = 6.  Additionally,
                                        both numeric values and expressions formed from the above supported
                                        keywords can be used.  E.g. for a Fit_Formula = 'gaus(0)' the initial
                                        guess of '12.3, MEAN, 2.*SIGMA+23.5' can be assigned.

                Fit_Param_Limit_Max     string, as Fit_Param_IGuess but for the upper limit on fit parameters.

                Fit_Param_Limit_Min     string, as Fit_Param_IGuess but for the upper limit on fit parameters.

                Fit_Param_Map           string, words to help the user track the meaning of the fit parameter.
                                        The order follows the ordering schema described in Fit_Param_IGuess.
                                        NOTE: at least one parameter must be given the value "PEAK" from the
                                        supported keywords.  The vertical error-bar also comes from the fit
                                        error on this parameter (open issue to move to "SIGMA", "HWHM",
                                        or "FWHM").

                Fit_Range               string, as Fit_Param_IGuess but for the fit range.  NOTE: must supply
                                        at least two parameters.  If more then two parameters are supplied
                                        only those that evaluate to the maximum and minimum are used.

        # 4.e.ii.V HEADER PARAMETERS - HISTO_INFO
        # --------------------------------------------------------

        The user at runtime is able to specify the data members of the TH1 objects that will be created
        (e.g. number of bins, TName, etc...).  However, unlike amoreSRS these histograms are hard coded and
        the user cannot choose at runtime to make new distributions.  If a distribution you would like to
        see does not already exist please navigate to:

            https://github.com/bdorney/CMS_GEM_Analysis_Framework/issues

        and submit a request; or if you are a developer implement the distribution yourself and then submit
        a pull request to the repository (https://github.com/bdorney/CMS_GEM_Analysis_Framework/pulls).

        The table below describes the allowed input fields and their data types.

            The following parameters are supported:
            #		<FIELD>             <DATA TYPE, DESCRIPTION>

                Histo_Name              string, type of histogram you are specifying information for.  NOTE:
                                        this entry should appear as the first line below the section
                                        header ("BEGIN_HISTO_INFO") and only entries from the following
                                        set are allowed: {"clustADC", "clustMulti", "clustPos", "clustSize",
                                        "clustTime", "hitADC", "hitPos", "hitTime"}.

                Histo_XTitle            string, title of the x-axis, full TLatex style entires are supported.
                                        Explicitly "cluster position #left(#mum#right)" will result in a
                                        well formatted x-axis with parenthesis that are sized accordingly
                                        and proper Greek lettering.

                Histo_YTitle            string, as Histo_XTitle but for the y-axis.

                Histo_BinRange          float, two floats separated by a comma specifying the lowest and
                                        highest values of the x-axis.  In the case of Histo_Name = clustPos
                                        this field is automatically set based on input from amoreSRS mapping file.

                Histo_NumBins           integer, number of bins in the histogram.  In the case of
                                        Histo_Name = clustPos this field is automatically set based on input
                                        from amoreSRS mapping file.

        # 4.e.ii.VI Example Config File
        # --------------------------------------------------------

        An example config file is shown below (feel free to remove the leading tabs, they are shown here
        just for readablility):

            [BEGIN_ANALYSIS_INFO]
                [BEGIN_UNIFORMITY_INFO]
                    #Selection Cuts
                    ####################################
                    Cut_ADC_Min = '700';
                    Cut_ClusterMulti_Min = '0';
                    Cut_ClusterMulti_Max = '20';
                    Cut_ClusterSize_Min = '2';
                    Cut_ClusterSize_Max = '20';
                    Cut_ClusterTime_Min = '1';
                    Cut_ClusterTime_Max = '14';
                    #Selection Cuts - Hit
                    ####################################
                    Cut_HitMulti_Min = '0';
                    Cut_HitMulti_Max = '3072';
                    Cut_HitTime_Min = '1';
                    Cut_HitTime_Max = '14';
                    #Event Range
                    ####################################
                    Event_First = '0';
                    Event_Total = '-1';
                    #Requested Granularity
                    ####################################
                    Uniformity_Granularity = '8';
                    #Requested Tolerance on Uniformity
                    ####################################
                    Uniformity_Tolerance = '0.15';
                    [BEGIN_ADC_FIT_INFO]
                        #ADC Spectrum Fit Parameters - Brian (Lorentz + Poly BKG; Lorentz = CauchyDist in ROOT)
                        Fit_Formula = '[0]*TMath::CauchyDist(x, [1], [2])+pol4(3)';
                        Fit_Option = 'QM';
                        Fit_Param_Map = 'AMPLITUDE, PEAK, HWHM';
                        Fit_Param_IGuess = '127898, PEAK, 200';
                        Fit_Param_Limit_Min = '10, PEAK-0.1*PEAK, 10';
                        Fit_Param_Limit_Max = '700000, PEAK+0.1*PEAK, 800';
                        Fit_Range = '800, 6000';
                    [END_ADC_FIT_INFO]
                    [BEGIN_HISTO_INFO]
                        Histo_Name = 'clustADC';
                        Histo_XTitle = 'Cluster ADC';
                        Histo_YTitle = 'N';
                        Histo_BinRange = '0,15000';
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
                        Histo_BinRange = '0,10';  #Expects two numbers separated by a comma
                        Histo_NumBins = '10';
                    [END_HISTO_INFO]
                    [BEGIN_HISTO_INFO]
                        Histo_Name = 'clustTime';
                        Histo_XTitle = 'Time Bin';
                        Histo_YTitle = 'N';
                        Histo_BinRange = '0,23';  #Expects two numbers separated by a comma
                        Histo_NumBins = '23';
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
                        Histo_BinRange = '0,23';  #Expects two numbers separated by a comma
                        Histo_NumBins = '23';
                    [END_HISTO_INFO]
                [END_UNIFORMITY_INFO]
            [END_ANALYSIS_INFO]

        # 4.e.iii. Run Config File
        # --------------------------------------------------------

        The run config file expects a certain "nested-header" style. The format should look something like:

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

        Parameters found inside the "[BEGIN_RUN_INFO]" header expected to be entered in the following format:

            field_name = 'value';

        The field_name should be on the left followed by an equals sign "=" then the value should be enclosed
        in single quote marks "'".  A semicolon ";" ends the statement.  Tabs "\t" and spaces outside of the
        single quotes will be ignored, but will be preserved inside the single quotes.  Text after the ";" will
        also be ignored.

        Contrary to the "[BEGIN_RUN_INFO]" header the "[BEGIN_RUN_LIST]" header is simply a list of PFN of the
        input files to be analyzed by the call of the executable.  Again tabs "\t" and spaces will be ignored.

        The Uniformity::ParameterLoaderRun class understands the "#" character to indicate a comment; so
        it is possible to comment out lines in the Run Config file you create for ease of use.  The template
        run config file at the end of this subsection showns an example.

        The value of true is understood as being from the case-insensitive set {t, true, 1} while the value of
        false is understood as being from the case-insensitive set {f, false, 0}.

        # 4.e.iii.I HEADER PARAMETERS - RUN_INFO
        # --------------------------------------------------------

        The table below describes the allowed input fields and their data types.

            The following parameters are supported:
            #		<FIELD>             <DATA TYPE, DESCRIPTION>

                Config_Analysis             string, PFN of the input analysis configuration file.

                Config_Mapping              string, PFN of the input mapping configuration file.

                Detector_Name               string, the serial number of the detector (do not include special
                                            characters such as '/' but dashes '-' are allowed)

                Input_Is_Frmwrk_Output      boolean, set to true (false) if the input file/files is/are created
                                            by the CMS_GEM_Analysis_Framework (amoreSRS).  Note that if this
                                            option is set to true then Output_Individual must also be set to true.

                Output_File_Name            string, PFN of the output TFile.  Note that if Output_Individual is
                                            set to true and Input_Is_Frmwrk_Output is set to false then the PFN
                                            defined here is not used.  Instead the PFN of the input TFile, created
                                            by amoreSRS, is used but the "dataTree.root" ending of the file name is
                                            removed and replaced with "Ana.root".  If Input_Is_Frmwrk_Output is set
                                            to true then the PFN defined here is again not used.  Instead the PFN of
                                            the input TFile, created by CMS_GEM_Analysis_Framework, is used but the
                                            filename is appended with "NewAna.root".  This could be potentially
                                            improved in the future.

                Output_File_Option          string, the option for the output TFile taken from the standard set
                                            defined in the TFile documentation, e.g. "CREATE, NEW, READ,
                                            RECREATE, UPDATE"

                Output_Individual           boolean, setting to true produces one output file for each input file.
                                            Setting to false produces one output file that represents the entirity
                                            of the analysis of all input files.  Note that this should only be set
                                            to false if Input_Is_Frmwrk_Output is also set to false.

                Ana_Reco_Clusters           boolean, set to true if you would like to the framework to reconstruct
                                            clusters from input hits found in the amoreSRS input TFile. Setting to
                                            false takes the clusters from the input amoreSRS TFile.  Right now this
                                            field does nothing and is only a placeholder.

                Ana_Hits                    boolean, setting to true will tell the framework to perform the analysis
                                            of the input hits.

                Ana_Clusters                boolean, setting to true will tell the framework to perform the analysis
                                            of the input clusters.

                Ana_Fitting                 boolean, setting to true will tell the framework to fit the obtained
                                            distributions.  Note that Ana_Clusters must also be true for those
                                            distributions to be fitted.

                Visualize_Plots             boolean, setting to true will tell the framework to prepare several
                                            TCanvases after analyzing all input files (Output_Individual = false)
                                            or each input file (Output_Individual = true).

                Visualize_AutoSaveImages    boolean, setting to true will tell the framework to automatically create
                                            *.png and *.pdf files of all TCanvases stored in the "Summary" folder.
                                            The name of these files will match the TName of the corresponding TCanvas.
                                            They will be found in the working directory (the directory you execute
                                            the framework executable from).  If these files already exist they will
                                            be over-written.

                Visualize_DrawPhiLines      boolean, setting to true will tell the framework to draw lines on the
                                            summary TCanvases that show the iPhi segmentation.


        # 4.e.iii.II  HEADER PARAMETERS - RUN_LIST
        # --------------------------------------------------------

        This header contains a list of PFN of the input files.  It is expected that there is one line for per
        file.  White space, such as tabs '\t' and spaces ' ', is ignored when reading in these input files.
        For example:

            [BEGIN_RUN_LIST]
                ...
                ...
                /filepath/filename3.root
                /filepath/filename4.root
                ...
                ...
            [END_RUN_LIST]

        # 4.e.iii.III  Configuration Options
        # --------------------------------------------------------

        There are three modes that the analyzeUniformity executable can be used to analyze raw data taken
        with RD51 Scaleable Readout System and unpacked amoreSRS.

        The first mode is the "series" mode which will analyze all of the input files defined in the
        "[BEGIN_RUN_LIST]" header, one after another, created by either amoreSRS or the
        CMS_GEM_Analysis_Framework (but not a mix of both!).  The time of execution can vary depending on
        the number of input files and the number of events/slice granularity present in each of those files.
        However, care has been taken to maximimize performance while still maintaining modularity/flexibility
        in the design.  Here the "[BEGIN_RUN_INFO]" header can be configured to execute whatever level of
        the framework analysis is desired; but this will be applied to EACH of the input files defined in
        the "[BEGIN_RUN_LIST]" header.  If the need arises we can implement a way to assign a different
        analysis and mapping config file to each of the input files defined in the "[BEGIN_RUN_LIST]" header
        but right now this functionality is not foreseen.

        The second mode is the computing cluster mode; but to avoid confusion with charge clusters this is
        referred to as "grid" mode. Here the input run config file contains only a single input file in the
        "[BEGIN_RUN_LIST]" header and the "[BEGIN_RUN_INFO]" header is configured such that Ana_Fitting and
        Visualize_Plots are set to false.  The user uses a script/scheduler of their choice to launch their
        jobs to a computing cluster to analyze a set of input files in parallel (scripts to do this are not
        yet present in the repository but is foreseen).  Then after all jobs are finished and the outputs
        retrieved the user can merge the output files together (if running on amoreSRS input files) using
        the "hadd" command in ROOT.  Then this merged file can be reprocessed in series mode with
        Input_Is_Frmwrk_Output, Ana_Fitting, and Visualize_Plots set to true.

        The third mode is the "re-run" mode.  Here one can take a TFile that has been previously produced by
        the CMS_GEM_Analysis_Framework and reanalyze it after changing the fit parameters defined in the
        analysis configuration file.  Each run will result in a new TFile (independent from the input) that
        has the updated results.  This allows the user to more rapidly study variations in parameters without
        having to waste time performing the base selection (which may not need to change).

        Example configuration files illustrating these options are provided in the sections below.

        # 4.e.iii.IV  Example Config File - Mode: Series
        # --------------------------------------------------------

        Two example files here are presented.

        The first example illustrates a series run in which the entire analysis is requested on a list of
        input TFile's create by amoreSRS.  Here the Output_Individual is set to false to create one output
        file representing the results of the analysis on all input files. Changing Output_Individual to true
        will produce one output file per input file.  The example is as follows:

            [BEGIN_RUN_INFO]
                #Config Files
                ####################################
                Config_Analysis = 'config/configAnalysis.cfg';
                Config_Mapping = 'config/GE7MappingCMScernData2016.cfg';
                #Detector
                ####################################
                Detector_Name = 'DETECTORNAME';
                #Input Config
                ####################################
                Input_Is_Frmwrk_Output = 'false';   #indicates we are running on input created by amoreSRS
                #Output Config
                ####################################
                Output_File_Name = 'myOutputFile.root';
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
                /base_dir/sub_dir/sub_dir/filename1.root
                /base_dir/sub_dir/sub_dir/filename2.root
                /base_dir/sub_dir/sub_dir/filename3.root
                /base_dir/different_dir/filename4.root
                /different_base_dir/filename5.root
            [END_RUN_LIST]

        Here leading tabs are shown just for convenience and can be kept/or omitted without consequence.

        The second example illustrates a series run in which only the final portion of the analysis (fitting
        and visualizing) is performed on an input TFile created by the CMS_GEM_Analysis_Framework.  Here
        Output_Individual is set to true, as it must be, and one output file for each input file will be
        produced.  The example is as follows:

            [BEGIN_RUN_INFO]
                #Config Files
                ####################################
                Config_Analysis = 'config/configAnalysis.cfg';
                Config_Mapping = 'config/GE7MappingCMScernData2016.cfg';
                #Detector
                ####################################
                Detector_Name = 'GE1/1-VII-L-CERN-0001';
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
                /base_dir/sub_dir/sub_dir/myPreviousFrmwrkOutput_Detector1.root
                /base_dir/sub_dir/sub_dir/myPreviousFrmwrkOutput_Detector2.root
                /base_dir/sub_dir/sub_dir/myPreviousFrmwrkOutput_Detector3.root
                /base_dir/different_dir/myPreviousFrmwrkOutput_Detector4.root
                /different_base_dir/myPreviousFrmwrkOutput_Detector5.root
            [END_RUN_LIST]

        Again the leading tabs are shown just for convenience and can be kept/or omitted without consequence.
        In this case the framework should create the following list of output files:

            /base_dir/sub_dir/sub_dir/myPreviousFrmwrkOutput_Detector1_NewAna.root
            /base_dir/sub_dir/sub_dir/myPreviousFrmwrkOutput_Detector2_NewAna.root
            /base_dir/sub_dir/sub_dir/myPreviousFrmwrkOutput_Detector3_NewAna.root
            /base_dir/different_dir/myPreviousFrmwrkOutput_Detector4_NewAna.root
            /different_base_dir/myPreviousFrmwrkOutput_Detector5_NewAna.root

        Pay special attention to the fact that these files will not necessarily be found in the directory you
        are calling the executable from but in the directory the input file is found in.

        # 4.e.iii.V  Example Config File - Mode: Grid
        # --------------------------------------------------------

        Grid mode is really designed for running the analysis on multiple input TFiles, created by amoreSRS,
        in parallel.  One could use this option when running on multiple input TFiles created by the framework
        but the increase in analysis speed would be small in comparison since usually you are only interested
        in checking a new set of fit parameters on the previously obtained data.

        Ideally you should submit this with the provided script/runMode_Grid.sh script, included in the
        repository, to a fast queue such as the 8 natural minute (8nm) or 1 natural hour (1nh) queue.
        The example config file is shown as:

            [BEGIN_RUN_INFO]
                #Config Files
                ####################################
                Config_Analysis = 'config/configAnalysis.cfg';
                Config_Mapping = 'config/GE7MappingCMScernData2016.cfg';
                #Input Config
                ####################################
                Input_Is_Frmwrk_Output = 'false';   #indicates we are running on input created by amoreSRS
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

        Again leading tabs are shown just for convenience and can be kept/or omitted without consequence.

        # 4.e.iii.VI  Example Config File - Mode: Re-Run
        # --------------------------------------------------------

        The re-run mode is designed to allow a user to change the fit parameters defined in their analysis
        config file and re-run on an input TFile previously produced by the framework.  This saves significant
        time when tweaking the fit parameters being applied to a given input file since the selection does
        not have to be repeated.  Obviously this mode should not be applied to input TFiles produced by amoreSRS.
        The example config file is given below:

            [BEGIN_RUN_INFO]
                #Config Files
                ####################################
                Config_Analysis = 'config/configAnalysis.cfg';
                Config_Mapping = 'config/GE7MappingCMScernData2016.cfg';
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

        Again the leading tabs are shown just for convenience and can be kept/or omitted without consequence.
        In this case the framework should create the following list of output files:

            /base_dir/sub_dir/sub_dir/myPreviousFrmwrkOutput_Detector1_NewAna.root

        Again pay special attention to the fact that these files will not necessarily be found in the directory you
        are calling the executable from but in the directory the input file is found in.

        Astute readers will note this is identical to the series mode example 2 with just one input file.
        This is true; however, I felt the explicit example could prove useful.

    # 4.f. Output ROOT File
    # --------------------------------------------------------

    The output ROOT file produced by classes inheriting from AnalyzeResponseUniformity will contain the
    TObjects described in Sections 4.b.i, 4.b.iii, and 4.d.ii.  The output file will have a repeating file
    structure.  For each SectorEta defined (i.e. "DET" line in the amoreSRS mapping config file) there
    will be one TDirectory named "SectorEtaX" where X is an integer.  Those TObject's stored in the
    Uniformity::SectorEta struct will be stored directly in this "SectorEtaX" TDirectory; and obviously
    they will represent only distributions from that iEta value.  The TName's for each TObject here
    will include the string "_iEtaX_" to ensure they are unique.

    Within each "SectorEtaX" folder will be nbConnect number of TDirectory's labeled "SectorPhiY" for
    Y = {1, 2, 3}.  Similarly to the above, the TObject's stored in the Uniformity::SectorPhi struct
    will be stored directly in this "SectorPhiY" TDirectory; they will represent only distributions
    from this (iEta, iPhi) value.  Again, the TName's for each TObject here will include the string
    "_iEtaXiPhiY_" to ensure they are unique.

    Within each "SectorPhiY" folder there will exist AnalysisSetupUniformity::iUniformityGranularity
    number of TDirectory's labeled "SliceZ" where Z is an integer from 1 to
    AnalysisSetupUniformity::iUniformityGranularity.  Similarly, the TObject's stored in
    Uniformity::SectorSlice will be stored directly in this "SliceZ" TDirectory; they will only
    represent distributions from this (iEta, iPhi, Slice) value.  Again, the TName's for each TObject
    here will include the string "_iEtaXiPhiYSliceZ_" to ensure tehy are unique.

    One top level TDirectory named "Summary" will also exist.  This folder will store a set of histograms
    for each cluster/hit observable.  The contents of these histograms is simply the sum of the
    corresponding SectorEtaX histograms (e.g. TH1::Add() method).

    Additionally the VisualizeUniformity class places additional TObjects (e.g. TCanvas, TMultiGraph, etc...)
    to assist the analyst in making the "pass/fail" statement.  These are desribed below.

        # 4.f.i. "Segmented" Plots Stored in "Summary" folder
        # --------------------------------------------------------

        Several TCanvas objects with TNames of the form:

            canv_<Detector_Name>_<Observable>_AllEta_Segmented

        will be stored in the folder.  Here the "Detector_Name" is the parameter defined in the given
        configRun.cfg file and "Observable" comes from the set {ClustPos, ClustADC, ClustSize, ClustTime}.

        These will show a TCanvas with an array of TPads placed in a 2x4 grid (columns-by-rows).  Each TPad
        will have ieta index written in the upper left corner of the pad and have the corresponding TObject
        from this iEta value drawn on the pad.

        # 4.f.ii. "Dataset" Plots Stored in "Summary" folder
        # --------------------------------------------------------

        Several TCanvas objects with TNames of the form:

            canv_<Detector_Name>_<Observable>Dataset_AllEta

        will be stored in the folder along with matching TH1F objects for each TCanvas with the
        TName of the form:

            h_Summary_<Observable>Dataset

        Here the "Detector_Name" is the parameter defined in the given
        configRun.cfg file and "Observable" comes from the set {ResponseFitPkPos}, to be expanded at a later date.

        The x-axis will be the <Observable> in question (e.g. for ResponseFitPkPos this will be the cluster ADC
        of the peak determined from the fit).  The Y-axis will be counts.  These canvases show the distribution
        of the observable in questin over the entire detector.  The TH1F in question will always have the bin
        range [Avg - 5 * StdDev, Avg + 5 * StdDev) with a bin width of 0.25 * StdDev.  Here "Avg" is the average
        of the dataset and "StdDev" is the dataset's standard deviation.  This TH1F will also be automatically
        fit with a Gaussian whose mean and sigma parameters will be written on the TPad.  The percent error of
        the dataset, defined as sigma / mean from the Gaussian, will also be displayed on the TPad.  This offers
        an "at a glance" look at the total distribution for a given observable and may help understand an immediate
        pass/fail condition.

        # 4.f.iii. 1D Fit Summary Plots Stored in "Summary" folder
        # --------------------------------------------------------

        Several TCanvas objects with TNames of the form:

            canv_<Detector_Name>_<FitObservable>_AllEta

        will be stored in the folder along with matching TMultiGraph objects for each TCanvas with the
        TName of the form:

            mgraph_<Detector_Name>_<FitObservable>_AllEta

        Here the "Detector_Name" is the parameter defined in the given configRun.cfg file. The "FitObservable"
        is from the set {ResponseFitChi2, ResponseFitPkPos, ResponseFitPkRes} for the normalized Chi2 value
        of the fit, determined peak position, and determined peak resolution (resolution = FWHM / position),
        respectively.

        # 4.f.iv. 2D Fit Trapezoidal Map Plots Stored in "Summary" folder
        # --------------------------------------------------------

        Several TCanvas objects with TNames of the form:

            canv_<Detector_Name>_<FitObservable>2D_AllEta

        will be stored in the folder along with matching TGraph2D objects for each TCanvas with the
        TName of the form:

            g2D_<Detector_Name>_<FitObservable>_AllEta

        Note in the case of the TCanvas a "2D" is placed in the TName to distinguish it from the 1D case.
        Here the "Detector_Name" is the parameter defined in the given configRun.cfg file. The "FitObservable"
        is from the set {ResponseFitPkPos, ResponseFitPkPosNormalized, ResponseFitPkRes,
        ResponseFitPkResNormalized}.  For the "Normalized" cases the z-axis at every point will be the point
        divided by the mean of the dataset formed by all points of the FitObservable (e.g. z / z_avg);

        These plots may take some time to load.  This is due to the rendering that is done by ROOT; be
        patient.  Consider transfering the file to your local machine if it is not already.  Once they load
        the plots will show a 3D plot of the detector.  The xy-plane will be the trapezoidal active area
        of the detector and the Z-axis will be hte FitObservable.

    # 4.g. Source Code Name Conventions
    # --------------------------------------------------------

    Developers are asked to adhere to the naming conventions below when working on the code base.  This
    improves readability and lowers the chance that ambiguity enters into the development.

        # 4.g.i. STL Objects
        # --------------------------------------------------------

        For those classes/data types/containers present in the STL the following naming conventions
        should be used:

            #		<TYPE>             <CONVENTION>
                    char                The character 'c' should start the object name, e.g. "cName".

                    int                 The character 'i' should start the object name, e.g. "iNumBins"

                    double              The character 'd' should start the object name, e.g. "dPeakPos"

                    float               The character 'f' should start the object name, e.g. "fPeakPos"

                    map                 For a std::map<T1 key, T2 val> the sequence 'map_' should start the
                                        object name followed by the character/sequence for type T2 if a
                                        convention already exists for it.  Right now T1 is mostly int type
                                        and is not specified.  If no convention for T2 exists use a starting
                                        sequence that is easily identifiable. e.g. "map_strName"
                                        or "map_hClustHisto"

                    multiset            For a std::multiset<T> the sequence 'mset_' should start the object
                                        name followed by the character/sequence for type T if a convention
                                        already exists for it. if no convention for T exists use a starting
                                        sequnce that is easily identified as described in the std::map
                                        description above.

                    shared_ptr          For std::shared_ptr<T> follow the convention for type T.

                    short               The sequence 's' should start the object name, e.g. sADC

                    string              The sequence 'str' should start the object name, e.g. "strName"

                    vector              Follow the convention for std::map but the starting sequence 'vec_'
                                        should be used instead. e.g. "vec_strName" or "vec_hClustHistos"


        # 4.g.i. ROOT Objects
        # --------------------------------------------------------

        For those classes in ROOT that are the following listed classes, or inherit from the following
        listed classes, use the naming conventions outlined below:

            #		<TYPE>             <CONVENTION>
                    TH1                 The character 'h' should start the object name, e.g. hClustADC

                    TH2                 As TH1 but the physical observables of the y & x axes should be
                                        included in the object name in "y vs. x" format,
                                        e.g. hEta_ClustADC_v_ClustPos

                    TF1                 The sequence 'fit' should start the object name, e.g. fitSlice_ClustADC

                    TGraph              The character 'g' should start the object name, e.g. gClustADC

# 5. Known & Outstanding Issues
# ========================================================

    Please consult the list of known/outstanding issues from the list below. If you do not find your
    issue please navigate to:

        https://github.com/bdorney/CMS_GEM_Analysis_Framework/issues

    And submit a new issue which outlines the problem, the commands exectued, the output error messages
    and a minimal piece of code to repeat the problem.  We will then try to get back to you with a solution.

    ********ISSUE********
    Compiling on MAC OS v10.9.X with g++ (GCC) version 4.9.2 20141029 fails.  This is due to the
    default C++ library in apple software.  Before 10.9.X the default was "libstdc++" but after 10.9.X
    it has been renamed to "libc++".  We have provided a make file for the clang compiler ("Makefile.clang)
    but do not support this computing environemnt since lxplus is so readily accessible and configurable.

    ********ISSUE********
    Running the analyzeUniformity executable in a MAC OS environment hangs indefinitely or seg faults
    when trying to parse the input analysis config file.  This appears to be due to the implementation of
    the standard library in MAC OS.  Specifically in the Timing::getlineNoSpaces() function declared in
    $GEM_BASE/include/TimingUtilityFunctions.h and implemented in $GEM_BASE/src/TimingUtilityFunctions.cpp.
    From what we can tell iterator operation e.g. strString.begin() + iNum seems to cause either a crash
    out right or the stl algorithm std::remove to exit the iterator range given and continue until the
    computer runs out of memory.  Our recommended solution is to use the linux computing environment
    specified above.

    ********ISSUE********
    Running analyzeUniformity executable on linux crashes when parsing the input analysis config file.
    This is a very rare occurrence and it seems to be again coming from Timing::getlineNoSpaces();
    function declared in $GEM_BASE/include/TimingUtilityFunctions.h and implemented in
    $GEM_BASE/src/TimingUtilityFunctions.cpp.  It is believed that for some reason the function tries to
    remove space or tab characters that are not there and return a null pointer causing the seg fault.
    If you identify the line of the input analysis config file the crash occurs at try adding or subtracting
    a few spaces or tabs and then re-running.  This has been seen to solve the issue in the past.  If a
    developer can come up with a better implementation of Timing::getlineNoSpaces() this might solve
    both this issue and the above issue.

    ********ISSUE********
    Runing analyzeUniformity gives error messages when parsing the input analysis config file stating
    it does not recognize a given (field, value) pair even though the field is listed in this README 
    file and have double checked that it is spelled correctly:

        strLine: = Cut_ClusterSize_Min='2';
        ParameterLoaderAnaysis::loadAnalysisParametersUniformity() - Error!!! Parameter Not Recognizd:
        ParameterLoaderAnaysis::loadAnalysisParametersUniformity() - 	Field = CUT_CLUSTERSIZE_MIN
        ParameterLoaderAnaysis::loadAnalysisParametersUniformity() - 	Value = 2

    We are puzzled by this also. Upon inspecting the output histograms it does appear that the specified
    selection cut is correctly applied.  Need to spend sometime with just ParameterLoaderAnalysis to
    understand what is going on.  Open issue.

    ********ISSUE********
    When attempting to run the executable analyzeUniformity I get the following error:

        ./analyzeUniformity: error while loading shared libraries:
        libSpectrum.so: cannot open shared object file: No such file or directory

    You did not run the setup script to initialize the computing environment.  Execute from the base 
    directory of the repository the following command:

        source scripts/setup_CMS_GEM.sh

    Now attempt to re-run the executable.

    ********ISSUE********
    When attempting to compile I get the following error (or similar):

        g++ -g3 -O0 `/bin/root-config --cflags --glibs --libs` -std=c++11 -I include/ -I/include/ -c src/DetectorMPGD.cpp -o src/DetectorMPGD.o  -L /lib/ -lSpectrum
        /bin/sh: /bin/root-config: No such file or directory
        cc1plus: error: unrecognized command line option "-std=c++11"
        make: *** [src/DetectorMPGD.o] Error 1

    You most likely did not run the setup script to initialize the computing environement.  Execute from
    the base directory of the repository the following command:

        source scripts/setup_CMS_GEM.sh

    Now attempt to recompile.

    ********ISSUE********
    When attempting to execute:

        source scripts/setup_CMS_GEM.sh

    The following error message is returned:

        gcc_config_version=4.8.4: Command not found.
        LCGPLAT=x86_64-slc6-gcc48-opt: Command not found.
        LCG_lib_name=lib64: Command not found.
        LCG_arch=x86_64: Command not found.
        BASH: Undefined variable.

    This has been traced to your $SHELL environment.  It seems the setup_CMS_GEM.sh script does not
    execute properly if you are not using zsh.  While we explore solutions to this problem we recommend
    running the framework using zsh.  We apologize for this inconvenience.
