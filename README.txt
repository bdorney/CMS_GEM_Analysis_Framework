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
    4. Documentation
        4.a. Namespaces
        4.b. Class Map
            4.b.i.      AnalyzeResponseUniformity
            4.b.ii.     ClusterSelector
            4.b.iii.    DetectorMPGD
            4.b.iv.     ParameterLoaderAmoreSRS
            4.b.v.      ParameterLoaderAnalysis
            4.b.vi.     VisualizeUniformity
        4.c. Utilities
            4.c.i   Timing
            4.c.ii  Uniformity
        4.d. Types
            4.d.i   Timing
            4.d.ii  Uniformity
        4.e. Configuration Files
            4.e.i   AMORE Mapping Config File
            4.e.ii  Analysis Config File
                4.e.ii.I    HEADER PARAMETERS - ANALYSIS_INFO
                4.e.ii.II   HEADER PARAMETERS - TIMING_INFO
                4.e.ii.III  HEADER PARAMETERS - UNIFORMITY_INFO
                4.e.ii.IV   HEADER PARAMETERS - ADC_FIT_INFO
                4.e.ii.V    HEADER PARAMETERS - HISTO_INFO
                4.e.ii.VI   Example Config File
        4.f. Output ROOT File
        4.g. Source Code Name Conventions
            4.g.i   STL Objects
            4.g.ii  ROOT Objects
    5. Known & Outstanding Issues

# 1. Contributors & License
# ========================================================

    Contributors: S. Colafranceschi, B. Dorney, and J. Merlin

    This package has been designed by B. Dorney with the help of J. Merlin & S. Colafranceschi.
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

        cd CMS_GEM_Analysis_Framework

        source scripts/setup_CMS_GEM.sh

        make -f Makefile.gpp clean

        make -f Makefile.gpp

    The repository is now compiled.  Additionally the base directory of the repository
    has been exported to the shell variable "$GEM_BASE".

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

        ./analyzeUniformity <PFN of AMORE Mapping File> <PFN of Analysis Config File> <PFN input ROOT file> <PFN of output ROOT file> <Option for output ROOT file>

    Here the physical file name (PFN) represents the full path+filename to the file in question.
    The two configuration files are described in Section 4.e.  The input ROOT file is a file
    produced by either AMORE or AMORE_ZS with ROOTDATATYPE = CLUSTERS_ONLY.  The contents and
    layout of the output root file are described in Section 4.f.  The option for the output ROOT
    file are the standard set from the TFile documentation, e.g. "CREATE, NEW, READ, RECREATE, UPDATE"

    Two example config files have been provided in the default repository.  A usage example is given as:

    ./analyzeUniformity config/GE7MappingCMScernData2016.cfg config/configAnalysis.cfg input.root output.root CREATE

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

        -None for now

    Friendship relations:

        AnalyzeResponseUniformity and ParameterLoaderAmoreSRS are both friend classes to DetectorMPGD

        AnalyzeResponseUniformity -> DetectorMPGD <- ParameterLoaderAmoreSRS

    Interactions:

        Classes ParameterLoaderAmoreSRS, ClusterSelector, and AnalyzeResponseUniformity all act on an
        object of DetectorMPGD.

        The ParameterLoaderAnalysis class interacts with objects of the ClusterSelector and
        AnalyzResponseUniformity classes.

        ParameterLoaderAmoreSRS -> creates a DetectorMPGD object
        ParameterLoaderAnalysis -> sets up the user specified analysis; this info is passed separately to ClusterSelector & AnalyzeResponseUniformity.
        ClusterSelector -> Assigned an input DetectorMPGD object the opens an input root file and performs the cluster selection; selected clusters are stored based on their location in the DetectorMPGD object.
        AnalyzeResponseUniformity -> Acts on a DetectorMPGD object that has stored clusters and performs the analysis
        VisualizeUniformity -> Takes the raw plots produced by AnalyzeResponseUniformity and presents them in a user friendly manner.

        # 4.b.i. AnalyzeResponseUniformity
        # --------------------------------------------------------

        Coming "soon"

        # 4.b.ii. ClusterSelector
        # --------------------------------------------------------

        Coming "soon"

        # 4.b.iii. DetectorMPGD
        # --------------------------------------------------------

        Coming "soon"

        # 4.b.iv. ParameterLoaderAmoreSRS
        # --------------------------------------------------------

        Coming "soon"

        # 4.b.v. ParameterLoaderAnalysis
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

        The AnalysisSetupUniformity struct stores user input defined in the Analysis Config file.  This
        struct has one instance of a HistoSetup struct for each cluster physical obserable (e.g. ADC,
        position, etc...).  Additionally this struct has one instance of a SelParamClusters struct; which
        stores user input defined in the Analysis Config file for cluster selection.

        Data members of Uniformity::AnalysisSetupUniformity are:

            AnalysisSetupUniformity::iEvt_First             //First event of TTree, produced by AMORE with ROOTDATATYPE = CLUSTERS_ONLY, to process.
            AnalysisSetupUniformity::iEvt_Total             //Total events of a TTree, produced by AMORE with ROOTDATATYPE = CLUSTERS_ONLY, to process.
            AnalysisSetupUniformity::iUniformityGranularity //The level of granularity the analysis will be carried out on  all iPhi sectors of a DetectorMPGD object (e.g. a value of 128 means at the strip level, a value of 2 means two groups of 64 strips).
            AnalysisSetupUniformity::fUniformityTolerance   //The requested tolerance on the gain uniformity

            AnalysisSetupUniformity::histoSetup_clustADC    //HistoSetup obejct for specifying cluster ADC TH1 & TF1 objects
            AnalysisSetupUniformity::histoSetup_clustMulti  //HistoSetup obejct for specifying cluster multiplicity TH1 objects
            AnalysisSetupUniformity::histoSetup_clustPos    //HistoSetup obejct for specifying cluster position TH1 objects
            AnalysisSetupUniformity::histoSetup_clustSize   //HistoSetup obejct for specifying cluster size TH1 objects
            AnalysisSetupUniformity::histoSetup_clustTime   //HistoSetup obejct for specifying cluster time bin TH1 objects

            AnalysisSetupUniformity::selClust               //SelParamClusters object specifying cluster selection cuts

        Data members of Uniformity::SelParamClusters are:

            SelParamClusters::iCut_ADCNoise     //Cluster rejected if ADC LESS than value
            SelParamClusters::iCut_MultiMin     //EVENT rejected if cluster multiplicity LESS than or equal to value
            SelParamClusters::iCut_MultiMax     //EVENT rejected if cluster multiplicity GREATER than or equal to value
            SelParamClusters::iCut_SizeMin      //Cluster rejected if cluster size LESS than value
            SelParamClusters::iCut_SizeMax      //Cluster rejected if cluster size GREATER than value
            SelParamClusters::iCut_TimeMin      //Cluster rejected if cluster time bin LESS than value
            SelParamClusters::iCut_TimeMax      //Cluster rejected if cluster time bin GREATER than value

        The Uniformity::Cluster struct stores information relating to one reconstructed cluster
        stored in the TTree created by AMORE with ROOTDATATYPE = CLUSTERS_ONLY.  Data members
        of Uniformity::Cluster are:

            Cluster::iPos_Y     //Distance in mm from wide base of trapezoid to cluster (e.g. vertical midpoint of iEta sector)
            Cluster::fPos_X     //Horizontal position within iEta sector (used to assign to correct iPhi sector)
            Cluster::fADC       //ADC value of cluster
            Cluster::iSize      //Number of strips in cluster
            Cluster::iTimeBin   //Time bin of cluster

        Note data types of Uniformity::Cluster (e.g. int, float, etc...) should closely match what AMORE stores
        in TTree (e.g. ADC is intrinscially integer physically, but it is defined as a float in AMORE );

        The Uniformity::SectorEta struct represents one iEta row of a detector. Each instance of a
        Uniformity::SectorEta will store three instances of a Uniformity::SectorPhi struct (hard coded).
        Each instance of a Uniformity::SectorPhi struct will store
        Uniformity::AnalysisSetupUniformity::iUniformityGranularity number of Uniformity::SectorSlice
        struct instances.  An instance of a Uniformity::DetectorMPGD class will store a number of
        instances of Uniformity::SectorEta objects.

        The data members of the Uniformity::SectorEta struct are:

            SectorEta::fPos_Y                       //Vertical Midpoint, in mm, of iEta row from wide base of trapezoid
            SectorEta::fWidth                       //Width of iEta sector, in mm, at SectorEta::fPos_Y;
            SectorEta::map_sectorsPhi               //Container storing three instances of SectorPhi objects
            SectorEta::vec_fClustADC_Fit_PkPos      //Container storing peak position from Clust ADC Fit
            SectorEta::vec_fClustADC_Fit_PkWidth    //Container storing peak width from Clust ADC Fit (not yet implemented)
            SectorEta::vec_fClustADC_Spec_PkPos     //Container storing peak position from TSpectrum::Search() & TSpectrum::GetPositionX()
            SectorEta::gEta_ClustADC_Fit_NormChi2   //std::shared_ptr of a TGraphErrors storing NormChi2 of fits from all SectorSlice::hSlice_ClustADC
            SectorEta::gEta_ClustADC_Fit_PkPos      //std::shared_ptr of a TGraphErrors storing ADC spec peak position from fits of all SectorSlice::hSlice_ClustADC
            SectorEta::gEta_ClustADC_Fit_Failures   //As SectorEta::gEta_ClustADC_Fit_PkPos but for when the minimizer did not succeed in finding a minima
            SectorEta::gEta_ClustADC_Spec_NumPks    //std::shared_ptr of a TGraphErrors storing number of peaks found in the SectorSlice::hSlice_ClustADC histogram; based on TSpectrum::Search() and TSpectrum::GetNPeaks()
            SectorEta::gEta_ClustADC_Spec_PkPos     //As SectorEta::gEta_ClustADC_Fit_PkPos but from TSpectrum::Search() and TSpectrum::GetPositionX() instead of fitting
            SectorEta::hEta_ClustADC                //std::shared_ptr of a TH1F storing distribution of Cluster::fADC for all clusters in this SectorEta
            SectorEta::hEta_ClustMulti              //As SectorEta::hEta_ClustADC but for Cluster Multiplicity in the Event
            SectorEta::hEta_ClustPos                //As SectorEta::hEta_ClustADC but for Cluster::fPos_X
            SectorEta::hEta_ClustSize               //As SectorEta::hEta_ClustADC but for Cluster::iSize
            SectorEta::hEta_ClustTime               //As SectorEta::hEta_ClustADC but for Cluster::iTimeBin
            SectorEta::hEta_ClustADC_v_ClustPos     //std::shared_ptr of a TH2F storing Cluster::fADC vs. Cluster::fPos_X from all clusters in this SectorEta

        The data members of the Uniformity::SectorPhi struct are:

            SectorPhi::fPos_Xlow                //X lower boundary of iPhi sector, in mm, at SectorEta::fPos_Y;
            SectorPhi::fPos_Xhigh               //X upper boundary of iPhi sector, in mm, at SectorEta::fPos_Y;
            SectorPhi::fWidth                   //Width of iPhi sector, in mm, at SectorEta::fPos_Y;
            SectorPhi::map_slices               //Container storing Uniformity::AnalysisSetupUniformity::iUniformityGranularity number of Uniformity::SectorSlice objects
            SectorPhi::vec_clusters             //vector of stored Uniformity::Cluster located in this SectorPhi (iPhi value)
            SectorPhi::hPhi_ClustADC            //As SectorEta::hEta_ClustADC but only for this SectorPhi (iPhi value)
            SectorPhi::hPhi_ClustMulti          //As SectorEta::hEta_ClustMulti but only for this SectorPhi (iPhi value)
            SectorPhi::hPhi_ClustSize           //As SectorEta::hEta_ClustSize but only for this SectorPhi (iPhi value)
            SectorPhi::hPhi_ClustTime           //As SectorEta::hEta_ClustTime but only for this SectorPhi (iPhi value)
            SectorPhi::hPhi_ClustADC_v_ClustPos //As SectorEta::hEta_ClustADC_v_ClustPos but only for this SectorPhi (iPhi value)

        The data members of the Uniformity::SectorSlice struct are:

            SectorSlice::fPos_Center        //Location of the center of the slice, in mm, within the SectorPhi (iPhi value)
            SectorSlice::fWidth             //Width of the slice in mm
            SectorSlice::fitSlice_ClustADC  //std::shared_ptr of a TF1; used to fit SectorSlice::hSlice_ClustADC
            SectorSlice::hSlice_ClustADC    //As SectorEta::hEta_ClustADC but only for this SectorSlice

    # 4.e. Configuration Files
    # --------------------------------------------------------

    Two configuration files are require to run the analyzeUniformity exectuable.  The first is the AMORE
    mapping file.  The second is the Analysis Config file.  Examples of both files have been included in
    the $GEM_BASE/config directory

        # 4.e.i. AMORE Mapping Config File
        # --------------------------------------------------------

        The AMORE mapping config file is the file used by AMORE when analyzing data collected with the SRS
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
        stored.  In this way the AMORE mapping config file specifies the geometry of the DetectorMPGD object.

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

                Event_First             integer, the first event, of the TTree found in the input root file
                                        (created by AMORE with ROOTDATATYPE = CLUSTERS_ONLY), to start
                                        processing the cluster selection at.

                Event_Total             integer, total number of events to process from the Event_First of the
                                        TTree found in the input root file (created by AMORE with
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
        (e.g. number of bins, TName, etc...).  However, unlike AMORE these histograms are hard coded and
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
                                        "clustTime"}.

                Histo_XTitle            string, title of the x-axis, full TLatex style entires are supported.
                                        Explicitly "cluster position #left(#mum#right)" will result in a
                                        well formatted x-axis with parenthesis that are sized accordingly
                                        and proper Greek lettering.

                Histo_YTitle            string, as Histo_XTitle but for the y-axis.

                Histo_BinRange          float, two floats separated by a comma specifying the lowest and
                                        highest values of the x-axis.  In the case of Histo_Name = clustPos
                                        this field is automatically set based on input from AMORE mapping file.

                Histo_NumBins           integer, number of bins in the histogram.  In the case of
                                        Histo_Name = clustPos this field is automatically set based on input
                                        from AMORE mapping file.

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
                        #Here Histo_BinRange is set automatically based on input amore mapping file
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
                [END_UNIFORMITY_INFO]
            [END_ANALYSIS_INFO]

    # 4.e. Output ROOT File
    # --------------------------------------------------------

    The output ROOT file produced by the AnalyzeResponseUniformity class will contain the TObjects
    described in Sections 4.b.i, 4.b.iii, and 4.d.ii.  The output file will have a repeating file
    structure.  For each SectorEta defined (i.e. "DET" line in the AMORE mapping config file) there
    will be one TDirectory named "SectorEtaX" where X is an integer.  Those TObject's defined in the
    Uniformity::SectorEta struct will be stored directly in this "SectorEtaX" TDirectory; and obviously
    they will represent only distributions from that iEta value.  The TName's for each TObject here
    will include the string "_iEtaX_" to ensure they are unique.

    Within each "SectorEtaX" folder will be three TDirectory's labeled "SectorPhiY" for Y = {1, 2, 3}.
    Similarly to the above the TObject's defined in the Uniformity::SectorPhi struct will be stored
    directly in this "SectorPhiY" TDirectory; they will represent only distributions from this (iEta, iPhi)
    value.  Again, the TName's for each TObject here will include the string "_iEtaXiPhiY_" to ensure
    they are unique.

    Within each "SectorPhiY" folder there will exist AnalysisSetupUniformity::iUniformityGranularity
    number of TDirectory's labeled "SliceZ" where Z is an integer from 1 to
    AnalysisSetupUniformity::iUniformityGranularity.  Similarly, the TObject's defined in
    Uniformity::SectorSlice will be stored directly in this "SliceZ" TDirectory; they will only
    represent distributions from this (iEta, iPhi, Slice) value.  Again, the TName's for each TObject
    here will include the string "_iEtaXiPhiYSliceZ_" to ensure tehy are unique.

    The VisualizeUniformity class will offer additional TObjects (e.g. TCanvas, TH2F, etc...) to assist
    the analyst in making the "pass/fail" statement upon implementation.

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
                                        and is not specified.  If not convention for T2 exists use a starting
                                        sequence that is easily identifiable. e.g. "map_strName"
                                        or "map_hClustHisto"

                    shared_ptr          For std::shared_ptr<T> follow the convention for type T.

                    string              The sequence 'str' should start the object name, e.g. "strName"

                    vector              Follow the convention for std::map but the starting sequence 'vec_'
                                        should be used instead. e.g. "vec_strName" or "vec_hClustHistos"


        # 4.g.i. STL Objects
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
