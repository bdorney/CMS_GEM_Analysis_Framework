import sys, os
from ROOT import gROOT, TClass, TFile, TH1F, TIter, TKey

if __name__ == "__main__":

    #Import Options
    from AnalysisOptions import *
    from GainMapAnalysisSuite import PARAMS_GAIN
    from Utilities import getall

    #Specific Options
    parser.add_option("--ident", type="string", dest="strIdent",
		      help="Substring within a TObject's TName that corresponds to the divider current value",
		      metavar="strIdent")

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

    #Loop over all TObjects in this File
    list_IndepVar = []
    list_DepVar = []
    list_DepVar_Err = []
    for strPath, obj in getall(file_Input, ""):
	#Print the class name and path pair to user
	if options.debug:
	    print obj.ClassName(), strPath	

	#Skip all objects that are not TH1's or their daughters
	if obj.InheritsFrom("TH1") == False:
	    print "Skipping Current Object"
	    continue

	#Get this histogram & store the values of interest
	hInput = file_Input.Get(strPath)
	list_DepVar.append(hInput.GetMean())
	list_DepVar_Err.append(hInput.GetRMS())

	#Get the current values
	strName = hInput.GetName()
	list_NameFields = strName.split('_') 
	list_NameFields = [x for x in list_NameFields if options.strIdent in x ]
	#print list_CurrentVals

	#Store the current
	#fCurrent = float(list_NameFields[0].replace(options.strIdent, "."))
	#print "fCurrent = " + str(fCurrent)
	list_IndepVar.append(float(list_NameFields[0].replace(options.strIdent, ".")))

    #Reparameterize independent variable into gain & print data to user
    print "VAR_INDEP,VAR_DEP,VAR_DEP_ERR"
    for i in range(0,len(list_IndepVar)):
	#list_IndepVar[i]=np.exp(options.p0 * list_IndepVar[i] + options.p1)
	list_IndepVar[i]=params_gain.calcGain(list_IndepVar[i])         
	print str(list_IndepVar[i]) + "," + str(list_DepVar[i]) + "," + str(list_DepVar_Err[i])

    print "Finished"
