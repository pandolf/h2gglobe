#!/usr/bin/env python

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i","--infilename", help="Input file (binned signal from globe)")
parser.add_option("-o","--outfilename",default="cms_hgg_datacard.txt",help="Name of card to print (default: %default)")
parser.add_option("-p","--procs",default="ggh,vbf,wh,zh,tth",help="String list of procs (default: %default)")
parser.add_option("-c","--ncats",default=9,type="int",help="Number of cats (default: %default)")
parser.add_option("--photonSystCats",default="EBlowR9,EBhighR9,EElowR9,EEhighR9",help="String list of photon syst name (default: %default)")
parser.add_option("--toSkip",default="ggH:6,ggH:7,qqH:6,qqH:7",help="proc:cat which are to skipped (default: %default)")
parser.add_option("--isCutBased",default=False,action="store_true")
parser.add_option("--is2011",default=False,action="store_true")
(options,args)=parser.parse_args()

import ROOT as r
r.gROOT.ProcessLine(".L quadInterpolate.C+g")
from ROOT import quadInterpolate
r.gROOT.ProcessLine(".L $CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so")
r.gROOT.ProcessLine(".L ../libLoopAll.so")

# convert globe style to combine style process
combProc = {'ggh':'ggH','vbf':'qqH','wzh':'VH','wh':'WH','zh':'ZH','tth':'ttH','bkg_mass':'bkg_mass'}
globeProc = {'ggH':'ggh','qqH':'vbf','VH':'wzh','WH':'wh','ZH':'zh','ttH':'tth','bkg_mass':'bkg_mass'}
procId = {'ggH':0,'qqH':-1,'VH':-2,'WH':-2,'ZH':-3,'ttH':-4,'bkg_mass':1}

# setup
if options.is2011: sqrts=7
else: sqrts=8
if 'wzh'in options.procs.split(','):
	splitVH=False
if 'wh' in options.procs.split(',') and 'zh' in options.procs.split(','):
	splitVH=True
inFile = r.TFile.Open(options.infilename)
outFile = open(options.outfilename,'w')
bkgProcs = ['bkg_mass']
vbfProcs = ['qqH']
incCats = [0,1,2,3]
dijetCats = [4,5]
muonCat = [6]
eleCat = [7]
metCat = [8]
options.procs += ',bkg_mass'
options.procs = [combProc[p] for p in options.procs.split(',')]
options.toSkip = options.toSkip.split(',')
options.photonSystCats = options.photonSystCats.split(',')
inWS = inFile.Get('cms_hgg_workspace')
intL = inWS.var('IntLumi').getVal()

# info = [file,workspace,name]
dataFile = 'hgg.inputbkgdata_%dTeV_MVA.root'%sqrts
dataWS = 'cms_hgg_workspace'
bkgFile = 'hgg.inputbkgdata_%dTeV_MVA.root'%sqrts
bkgWS = 'cms_hgg_workspace'
sigFile = 'hgg.inputsig_%dTeV_MVA.root'%sqrts
sigWS = 'wsig_%dTeV'%sqrts
fileDetails = {}
fileDetails['data_obs'] = [dataFile,dataWS,'roohist_data_mass_$CHANNEL']
fileDetails['bkg_mass']	= [bkgFile,bkgWS,'pdf_data_pol_model_%dTeV_$CHANNEL'%sqrts]
fileDetails['ggH'] 			= [sigFile,sigWS,'hggpdfsmrel_%dTeV_ggh_$CHANNEL'%sqrts]
fileDetails['qqH'] 			= [sigFile,sigWS,'hggpdfsmrel_%dTeV_vbf_$CHANNEL'%sqrts]
if splitVH:
	fileDetails['WH'] 			=	[sigFile,sigWS,'hggpdfsmrel_%dTeV_wh_$CHANNEL'%sqrts]
	fileDetails['ZH'] 			=	[sigFile,sigWS,'hggpdfsmrel_%dTeV_zh_$CHANNEL'%sqrts]
else:
	fileDetails['VH'] 			=	[sigFile,sigWS,'hggpdfsmrel_%dTeV_wzh_$CHANNEL'%sqrts]
fileDetails['ttH'] 			= [sigFile,sigWS,'hggpdfsmrel_%dTeV_tth_$CHANNEL'%sqrts]

# theory systematics arr=[up,down]
pdfSyst = {}
pdfSyst['ggH'] = [0.076,-0.070]
pdfSyst['qqH'] = [0.026,-0.028]
if splitVH:
	pdfSyst['WH'] =  [0.042,-0.042]
	pdfSyst['ZH'] =  [0.042,-0.042]
else:
	pdfSyst['VH'] =  [0.042,-0.042]
pdfSyst['ttH'] = [0.080,-0.080]
scaleSyst = {}
scaleSyst['ggH'] = [0.076,-0.082]
scaleSyst['qqH'] = [0.003,-0.008]
if splitVH:
	scaleSyst['WH'] =  [0.021,-0.018]
	scaleSyst['ZH'] =  [0.021,-0.018]
else:
	scaleSyst['VH'] =  [0.021,-0.018]
scaleSyst['ttH'] = [0.041,-0.094]

# lumi syst
if options.is2011:
	lumiSyst = 0.045
else:
	lumiSyst = 0.044

# vtx eff
if options.is2011:
	vtxSyst = 0.005
else:
	vtxSyst = 0.02

# r9 syst (cut based only)
if options.isCutBased:
	if options.is2011:
		r9barrelSyst = 0.080 
		r9mixedSyst = 0.115
	else:
		r9barrelSyst = 0.040 
		r9mixedSyst = 0.065

# from globe
globeSysts={}
globeSysts['idEff'] = 'n_id_eff'
globeSysts['triggerEff'] = 'n_trig_eff'
if not options.isCutBased:
  globeSysts['phoIdMva'] = 'n_id_mva'
  globeSysts['regSig'] = 'n_sigmae'

# vbf uncertainties (different for 7 TeV?)
vbfSysts={}
if options.isCutBased:
	vbfSysts['CMS_hgg_JEC'] = [0.10,0.025] # [ggEffect,qqEffect]
	vbfSysts['CMS_hgg_UEPS'] = [0.28,0.072]
	vbfSysts['CMS_eff_j'] = [0.02,0.02]
	vbfSysts['CMS_hgg_JECmigration'] = [0.024,0.005] 
	vbfSysts['CMS_hgg_UEPSmigration'] = [0.063,0.034]
else:
	vbfSysts['CMS_hgg_JEC'] = [0.11,0.035] # [ggEffect,qqEffect]
	vbfSysts['CMS_hgg_UEPS'] = [0.26,0.08]
	vbfSysts['CMS_eff_j'] = [0.02,0.02]
	vbfSysts['CMS_hgg_JECmigration'] = [0.025,0.005] 
	vbfSysts['CMS_hgg_UEPSmigration'] = [0.045,0.010]

# lepton + MET systs (not done before for 7TeV)
eleSyst = {}
eleSyst['ggH'] = 0.00
eleSyst['qqH'] = 0.00
eleSyst['VH'] = 0.01
eleSyst['WH'] = 0.01
eleSyst['ZH'] = 0.01
eleSyst['ttH'] = 0.01
muonSyst = {}
muonSyst['ggH'] = 0.00
muonSyst['qqH'] = 0.00
muonSyst['VH'] = 0.01
muonSyst['WH'] = 0.01
muonSyst['ZH'] = 0.01
muonSyst['ttH'] = 0.01
metSyst = {}
metSyst['ggH'] = 0.15
metSyst['qqH'] = 0.15
metSyst['VH'] = 0.04
metSyst['WH'] = 0.04
metSyst['ZH'] = 0.04
metSyst['ttH'] = 0.04

def interp1SigmaFrom3Sigma(th1f_nom,th1f_down,th1f_up):
	downE = quadInterpolate(-1.,-3.,0.,3.,th1f_down.Integral(),th1f_nom.Integral(),th1f_up.Integral())
	upE = quadInterpolate(1.,-3.,0.,3.,th1f_down.Integral(),th1f_nom.Integral(),th1f_up.Integral())
	return [downE,upE]

def printPreamble():
	print 'Preamble...'
	if options.isCutBased:
		outFile.write('CMS-HGG datacard for parametric model - cut based analysis %dTeV \n'%sqrts)
	else:
		outFile.write('CMS-HGG datacard for parametric model - mass factorized analysis %dTeV \n'%sqrts)
	outFile.write('Auto-generated by h2gglobe/Macros/makeParametricModelDatacard.py\n')
	outFile.write('Run with: combine\n')
	outFile.write('---------------------------------------------\n')
	outFile.write('imax *\n')
	outFile.write('jmax *\n')
	outFile.write('kmax *\n')
	outFile.write('---------------------------------------------\n')
	outFile.write('\n')

def printFileOptions():
	print 'File opts...'
	for typ, info in fileDetails.items():
		outFile.write('shapes %-8s * %-30s %s:%s\n'%(typ,info[0],info[1],info[2]))
	outFile.write('\n')

def printObsProcBinLines():
	print 'Rates...'
	outFile.write('%-15s '%'bin')
	for c in range(options.ncats):
		outFile.write('cat%d '%c)
	outFile.write('\n')
	
	outFile.write('%-15s '%'observation')
	for c in range(options.ncats):
		outFile.write('-1 ')
	outFile.write('\n')
	
	outFile.write('%-15s '%'bin')
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			outFile.write('cat%d '%c)
	outFile.write('\n')
	
	outFile.write('%-15s '%'process')
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			outFile.write('%s '%p)
	outFile.write('\n')

	outFile.write('%-15s '%'process')
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			outFile.write('%d '%procId[p])
	outFile.write('\n')

	outFile.write('%-15s '%'rate')
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('1.0 ')
			else:
				outFile.write('%7.1f '%intL)
	outFile.write('\n')
	outFile.write('\n')

def printNuisParams():
	print 'Nuisances...'
	outFile.write('%-35s param 1.0 %5.4f\n'%('CMS_hgg_nuisancedeltafracright_%dTeV'%sqrts,vtxSyst))
	outFile.write('%-35s param 0.0 0.3333\n'%('CMS_hgg_globalscale'))
	if options.isCutBased:
		outFile.write('%-35s param 1.0 %5.4f\n'%('CMS_hgg_nuisancedeltar9barrel_%dTeV'%sqrts,r9barrelSyst))
		outFile.write('%-35s param 1.0 %5.4f\n'%('CMS_hgg_nuisancedeltar9mixed_%dTeV'%sqrts,r9mixedSyst))
	for phoSyst in options.photonSystCats:
		outFile.write('%-35s param 0.0 0.3333\n'%('CMS_hgg_nuisance%sscale'%phoSyst))
	for phoSyst in options.photonSystCats:
		outFile.write('%-35s param 0.0 0.3333\n'%('CMS_hgg_nuisance%ssmear'%phoSyst))
	outFile.write('\n')

def printTheorySysts():
	print 'Theory...'
	# scales
	for proc, uncert in scaleSyst.items():
		outFile.write('%-25s   lnN   '%('QCDscale_%s'%proc))
		for c in range(options.ncats):
			for p in options.procs:
				if '%s:%d'%(p,c) in options.toSkip: continue
				if p==proc:
					outFile.write('%5.3f/%5.3f '%(1.+uncert[1],1.+uncert[0]))
				else:
					outFile.write('- ')
		outFile.write('\n')
	outFile.write('\n')

	# pdfs
	for proc, uncert in pdfSyst.items():
		outFile.write('%-25s   lnN   '%('pdf_%s'%proc))
		for c in range(options.ncats):
			for p in options.procs:
				if '%s:%d'%(p,c) in options.toSkip: continue
				if p==proc:
					outFile.write('%5.3f/%5.3f '%(1.+uncert[1],1.+uncert[0]))
				else:
					outFile.write('- ')
		outFile.write('\n')
	outFile.write('\n')

def printLumiSyst():
	print 'Lumi...'
	outFile.write('%-25s   lnN   '%('lumi_%dTeV'%sqrts))
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('- ')
			else:
				outFile.write('%5.3f '%(1.+lumiSyst))
	outFile.write('\n')
	outFile.write('\n')

def printGlobeSysts():
	print 'Efficiencies...'
	for globeSyst, paramSyst in globeSysts.items():
		#print globeSyst, paramSyst
		outFile.write('%-25s   lnN   '%('CMS_hgg_%s'%paramSyst))
		for c in range(options.ncats):
			for p in options.procs:
				if '%s:%d'%(p,c) in options.toSkip: continue
				if p in bkgProcs:
					outFile.write('- ')
				else:
					th1f_nom = inFile.Get('th1f_sig_%s_mass_m125_cat%d'%(globeProc[p],c))
					th1f_up  = inFile.Get('th1f_sig_%s_mass_m125_cat%d_%sUp01_sigma'%(globeProc[p],c,globeSyst))
					th1f_dn  = inFile.Get('th1f_sig_%s_mass_m125_cat%d_%sDown01_sigma'%(globeProc[p],c,globeSyst))
					systVals = interp1SigmaFrom3Sigma(th1f_nom,th1f_dn,th1f_up) 
					outFile.write('%5.3f/%5.3f '%(systVals[0],systVals[1]))
		outFile.write('\n')
	outFile.write('\n')

def printVbfSysts():
	print 'Vbf...'
	# these always confuse me !
	for vbfSystName, vbfSystVals in vbfSysts.items():
		outFile.write('%-25s   lnN   '%vbfSystName)
		# the first one is migration between vbf cats (from loose to tight)
		if 'migration' in vbfSystName:
			looseDiJetEvCount={}
			otherDiJetEvCount={}
			for p in options.procs:
				looseDiJetEvCount[p] = 0.
				otherDiJetEvCount[p] = 0.
				for c in range(options.ncats):
					if '%s:%d'%(p,c) in options.toSkip: continue
					if p in bkgProcs: continue
					if c in dijetCats:
						th1f = inFile.Get('th1f_sig_%s_mass_m125_cat%d'%(globeProc[p],c))
						if c==len(incCats)+len(dijetCats)-1: looseDiJetEvCount[p] += th1f.Integral()
						else: otherDiJetEvCount[p] += th1f.Integral()
			# write lines
			for c in range(options.ncats):
				for p in options.procs:
					if '%s:%d'%(p,c) in options.toSkip: continue
					if p in bkgProcs:
						outFile.write('- ')
						continue
					elif p=='qqH': 
						thisUncert = vbfSystVals[1]
					else: 
						thisUncert = vbfSystVals[0]
					if c in dijetCats:
						if c==len(incCats)+len(dijetCats)-1: 
							outFile.write('%6.4f '%(1.+thisUncert))
						else:
							outFile.write('%6.4f '%((otherDiJetEvCount[p]-thisUncert*looseDiJetEvCount[p])/otherDiJetEvCount[p]))
					else:
						outFile.write('- ')
			outFile.write('\n')

		# the second one is migration from vbf to inclusive
		# find n events increase in vbf cat sum and work out what that translates to taking out from inc cats
		else:
			dijetEvCount={}
			incEvCount={}
			for p in options.procs:
				dijetEvCount[p] = 0.
				incEvCount[p] = 0.
				for c in range(options.ncats):
					if '%s:%d'%(p,c) in options.toSkip: continue
					if p in bkgProcs: continue
					th1f = inFile.Get('th1f_sig_%s_mass_m125_cat%d'%(globeProc[p],c))
					if c in incCats: incEvCount[p] += th1f.Integral()
					elif c in dijetCats: dijetEvCount[p] += th1f.Integral()
					else: continue
			# write lines
			for c in range(options.ncats):
				for p in options.procs:
					if '%s:%d'%(p,c) in options.toSkip: continue
					if p in bkgProcs:
						outFile.write('- ')
						continue
					elif p=='qqH': 
						thisUncert = vbfSystVals[1]
					else: 
						thisUncert = vbfSystVals[0]
					if c in incCats:
						outFile.write('%6.4f '%((incEvCount[p]-thisUncert*dijetEvCount[p])/incEvCount[p]))
					elif c in dijetCats:
						outFile.write('%6.4f '%(1.+thisUncert))
					else:
						outFile.write('- ')
			outFile.write('\n')

def printLepMetSysts():
	outFile.write('%-25s   lnN   '%('CMS_hgg_eff_muon'))
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('- ')
				continue
			if c in muonCat:
				outFile.write('%5.3f '%(1.+muonSyst[p]))
			else:
				outFile.write('- ')
	outFile.write('\n')
	outFile.write('%-25s   lnN   '%('CMS_hgg_eff_ele'))
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('- ')
				continue
			if c in eleCat:
				outFile.write('%5.3f '%(1.+eleSyst[p]))
			else:
				outFile.write('- ')
	outFile.write('\n')
	outFile.write('%-25s   lnN   '%('CMS_hgg_eff_MET'))
	for c in range(options.ncats):
		for p in options.procs:
			if '%s:%d'%(p,c) in options.toSkip: continue
			if p in bkgProcs:
				outFile.write('- ')
				continue
			if c in metCat:
				outFile.write('%5.3f '%(1.+metSyst[p]))
			else:
				outFile.write('- ')
	outFile.write('\n')

# __main__ here
printPreamble()
printFileOptions()
printObsProcBinLines()
printNuisParams()
printTheorySysts()
printLumiSyst()
printGlobeSysts()
printVbfSysts()
printLepMetSysts()
