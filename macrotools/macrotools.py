import subprocess

def callMacro(macroName, inputFile):
	macroCommand = "%s(\"%s\")" % (macroName, inputFile)
	subprocess.call(["root", "-l", "-q", macroCommand])
