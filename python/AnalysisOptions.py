from optparse import OptionParser

def hv_callback(option, opt, value, parser):
    setattr(parser.values, option.dest, value.split(','))

parser = OptionParser()

#debugging
parser.add_option("-d","--deubg", type="int", dest="debug",
                  help="Print debugging information", metavar="debug")

#input filename
parser.add_option("-f", "--file", type="string", dest="filename",
                  help="physical filename of input QC5_Resp_Uni analyzed file", metavar="filename", default="FrameworkOutput.root")

#gain parameters
parser.add_option("--gp0", type="float", dest="gain_P0",
                  help="P0 in G(x)=exp(P0*X+P1)", metavar="gain_P0")
parser.add_option("--gp0Err", type="float", dest="gain_P0_Err",
                  help="Error on P0 in G(x)=exp(P0*X+P1)", metavar="gain_P0_Err")
parser.add_option("--gp1", type="float", dest="gain_P1",
                  help="P1 in G(x)=exp(P0*X+P1)", metavar="gain_P1")
parser.add_option("--gp1Err", type="float", dest="gain_P1_Err",
                  help="Error on P1 in G(x)=exp(P0*X+P1)", metavar="gain_P1_Err")

#detector information
parser.add_option("-n", "--name", type="string", dest="det_name",
                  help="Serial Number of Detector, note this must be the value used in the input file",
                  metavar="det_name")
parser.add_option("-w", "--sectorSize", type="float", dest="det_sectSize",
                  help="Width of ieta row, in mm, supplied to --ieta", metavar="det_sectSize", default=319.48)
parser.add_option("--ieta", type="int", dest="det_ieta",
                  help="ieta for (ieta,iphi) sector that gain calibration measurement was performed in",
                  metavar="det_ieta", default=4)
parser.add_option("--iphi", type="int", dest="det_iphi",
                  help="iphi for (ieta,iphi) sector that gain calibration measurement was performed in",
                  metavar="det_iphi", default=2)
parser.add_option("--nbConnectRO", type="int", dest="det_nbconnect",
                  help="number of total readout connectors in ieta row that gain cal was performed in",
                  metavar="det_nbconnect", default=3)

#HV information
parser.add_option("--hvPoint", type="float", dest="hv_orig",
                  help="HV Point for which original QC5_Resp_Uni measurement was performed at", metavar="hv_orig")
#parser.add_option("-hvlist","--hvList", type="float",
#                  help="Comma separated list of HV values to calculate the gain map at",
#                  action="callback", callback=hv_callback
#                  dest = "hvlist")
parser.add_option("--hvlist", type="string", dest="hv_list",
                  help="Comma separated list of HV values to calculate the gain map at",metavar="hv_list")
