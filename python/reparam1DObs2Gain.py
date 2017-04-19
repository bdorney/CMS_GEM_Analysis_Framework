import sys, os
from ROOT import gROOT, TClass, TFile, TH1F, TIter, TKey

if __name__ == "__main__":

    #Import Options
    from AnalysisOptions import *
    from GainMapAnalysisSuite import PARAMS_GAIN

    #Get input options
    (options, args) = parser.parse_args()
    
    #Check to make sure user specified an input file name
    if options.filename is None:
        #print "options.filename = " + str(options.filename)
        print "You must specify the physical filepath of a Framework output file"
        print "Use option -f or --file"
        exit(1)
        pass

    #Check to make sure the user specified a gain curve
    if options.gain_P0 is None or options.gain_P1 is None:
        print "For G(x) = exp(P0 * x + P1)"
            print "P0 = " + str(options.gain_P0)
            print "P0_Err = " + str(options.gain_P0_Err)
            print "P1 = " + str(options.gain_P1)
            print "P1_Err = " + str(options.gain_P1_Err)
            print "You must specify values for P0 & P1"
            exit(1)
            pass

    #Check to make sure the user specified detector parameters
    if options.det_name is None:
        print "You must specify the detector name"
        print "Use option -n or --name"
        exit(1)
        pass

    #Define the gain parameters
    params_gain = PARAMS_GAIN(gain_p0=options.gain_P0,
                              gain_p0_err=options.gain_P0_Err,
                              gain_p1=options.gain_P1,
                              gain_p1_err=options.gain_P1_Err)

    #Load the input file
    file_Input	= TFile(str(options.filename),"READ","",1)
    TIter iterKeys(file_Input.GetListOfKeys())
    TKey keyCurrent

    #Loop Over the keys in the file
    while (keyCurrent = (TKey)iterKeys()):
        #Get the i^th object
        TClass objInput = gROOT.GetClass(keyCurrent.GetClassName())

        if !objInput.InheritsFrom("TH1"):
            print "Skipping Current Object"
            continue

        TH1 hInput(keyCurrent.ReadObj())

        print hInput.GetName()

    print "Finished"

