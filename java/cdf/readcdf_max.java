import com.cycling74.max.*;
import java.io.*;
import java.text.*;
import java.util.*;
import java.lang.reflect.*;
import gsfc.nssdc.cdf.*;
import gsfc.nssdc.cdf.util.*;

/**
 *  This program demonstrates how to read the contents of test.cdf created 
 *  by CreateCDF.java in this directory.
 */

public class readcdf_max extends MaxObject implements CDFConstants{

	//public static void main(String[] args) {
	public readcdf_max(){
		String[] args = null;
		String fileName = "/Applications/cdf32-dist/cdfjava/examples/test.cdf";
        int     maxVarNameLength = 22;

        try {
            CDF cdf = null; 
            //if (args.length == 0) {
                cdf  = CDF.open(fileName, READONLYoff);
		//}
		//else {
                //fileName = args[0];
                //cdf = CDF.open(fileName, READONLYoff);
		//}

            /************************************************************/
            /* If a decoding method is not specified when a CDF file is */
            /* opened, the CDF libaray knows what encoding method was   */
            /* used to create the CDF file.                             */
            /*                                                          */
            /* Decoding method should be specified only if one needs    */
            /* to translate data from one platform to another.          */ 
            /************************************************************/
            // cdf.selectDecoding(NETWORK_DECODING);

            /**********************************/
            /* Print out the file information */
            /**********************************/
            System.out.println("File Info\n"+
                               "=========================================");

            if (cdf.confirmReadOnlyMode() == READONLYon)
                System.out.println("CDF File:     "+cdf+" (READONLYon)");
            else {
                System.out.println("CDF File:     "+cdf+" (READONLYoff)");
            }

            System.out.println("Version:      "+cdf.getVersion());
            String cp = cdf.getCopyright();
            System.out.println("Copyright:    "+cp);
            System.out.println("Format:       "+CDFUtils.getStringFormat(cdf));
            System.out.println("Encoding:     "+
                               CDFUtils.getStringEncoding(cdf));
            System.out.println("Decoding:     "+
                               CDFUtils.getStringDecoding(cdf));
            System.out.println("Majority:     "+
                               CDFUtils.getStringMajority(cdf));
            System.out.println("Checksum:     "+
                               CDFUtils.getStringChecksum(cdf));
            System.out.println("numRvars:     "+cdf.getNumRvars());
            System.out.println("numZvars:     "+cdf.getNumZvars());
            System.out.println("numAttrs:     "+cdf.getNumAttrs()+
                               " ("+cdf.getNumGattrs()+" global, "+
                               cdf.getNumVattrs()+" variable)");
            System.out.println("Compression:  "+cdf.getCompression());
            if (cdf.getCompressionType() != 0)
              System.out.println("cPct:         "+cdf.getCompressionPct());
            System.out.println("Cache Size:   "+cdf.confirmCDFCacheSize());

            /**********************************************/
            /* Print out the Global Attribute information */
            /**********************************************/
            Attribute a;
            String    attrName = null;
            int       i;
            long      n = cdf.getNumGattrs();
            Vector    ga = cdf.getGlobalAttributes();
         
            System.out.println("\nGlobal Attributes ("+n+" attributes)\n"+
                               "=========================================");
            i = 0;
            for (Enumeration e = ga.elements() ; e.hasMoreElements() ;) {
                a = (Attribute) e.nextElement();
                n = a.getNumEntries(); 
                if (i == 0) {
                   attrName = a.getName();
                   if (n <= 1)
                       System.out.println (attrName+" ("+n+" entry):");
                   else
                       System.out.println (attrName+" ("+n+" entries):");
                }
                else {
                   String currAttrName = a.getName();;
                   if (currAttrName != attrName) {
                       if (n <= 1)
                          System.out.println (currAttrName+" ("+n+" entry):");
                       else
                          System.out.println (currAttrName+" ("+n+" entries):");
                   }
                } 
                i++;
                Vector ent = a.getEntries();
                for (Enumeration e1 =ent.elements() ; e1.hasMoreElements() ;) {
                    Entry entry = (Entry) e1.nextElement();
                    if (entry != null) {
                       if (entry.getID() == 11) 
                           ; // entry.delete();
                       else {
                          long eDataType = entry.getDataType();
                          System.out.print ("\t"+entry.getID()+" ("+
                                         CDFUtils.getStringDataType(eDataType)+
                                         "/"+entry.getNumElements()+
                                         "): \t");
                          Object data = entry.getData();
                          if (eDataType == CDF_EPOCH) {
                              CDFUtils.printData (
                                Epoch.encode(((Double) data).doubleValue()));
                          } else if (eDataType == CDF_EPOCH16) {
                              CDFUtils.printData (Epoch16.encode((double[])data));
                          } else
                              CDFUtils.printData (data);
                          System.out.println (" ");
                       }
                    }
                }
                System.out.println (" ");
            }

            /************************************************/
            /* Print out the Variable Attribute information */
            /************************************************/
            attrName = null;
            n = cdf.getNumVattrs();
            Vector  va = cdf.getVariableAttributes();

            System.out.println("\nVariable Attributes ("+n+" attributes)\n"+
                               "=========================================");
            i = 0;
            for (Enumeration e = va.elements() ; e.hasMoreElements() ;) {
                a = (Attribute) e.nextElement();
                if (i == 0) {
                    attrName = a.getName();
                    System.out.println (attrName+":");
                }
                else {
                    String currAttrName = a.getName();;
                    if (currAttrName != attrName)
                        System.out.println (currAttrName+":");
                }
                i++;
                Vector ent = a.getEntries();
                for (Enumeration e1 =ent.elements() ; e1.hasMoreElements() ;) {
                    Entry entry = (Entry) e1.nextElement();
                    if (entry != null) {
                       long eDataType = entry.getDataType();
                       Variable v = cdf.getVariable(entry.getID());
                       System.out.print ("\t"+v.getName()+" ("+
                                        CDFUtils.getStringDataType(eDataType)+
                                        "/"+entry.getNumElements()+
                                        "): ");

                       Object data = entry.getData();
                       if (eDataType == CDF_EPOCH) {
                           CDFUtils.printData (
                               Epoch.encode(((Double) data).doubleValue()));
                       } else if (eDataType == CDF_EPOCH16) {
                           CDFUtils.printData (Epoch16.encode((double[])data));
                       } else
                           CDFUtils.printData (data);
                       System.out.println (" ");
                    }
                }
                System.out.println (" ");
            }

            /**************************************/
            /* Print out the Variable information */ 
            /**************************************/
            String  varName, dataType;
            int     noOfBlanks;
            long    dt, numDims, blockingFactor;
            Object  padValue = null;

            n = cdf.getNumVars();
            Vector  vars = cdf.getVariables();

            System.out.println("\nVariable Information ("+n+" variables)\n"+
                               "=========================================");
            for (Enumeration e = vars.elements() ; e.hasMoreElements() ;) {
                Variable v = (Variable) e.nextElement();

                varName = v.getName();
                noOfBlanks = maxVarNameLength - varName.length();
                for (i=0; i < noOfBlanks; i++)
                    varName = varName + " ";

                long[] dimSizes = v.getDimSizes();
                dt = v.getDataType();
                dataType = CDFUtils.getStringDataType(dt);
                dataType = dataType + "/" + String.valueOf(v.getNumElements());
                noOfBlanks = 14 - dataType.length();
                for (i=0; i < noOfBlanks; i++)
                    dataType = dataType + " ";

                numDims = v.getNumDims();
                System.out.print (varName+dataType+ numDims+":[");
                for (i=0; i < numDims; i++) {
                    if (i > 0) System.out.print (",");
                    System.out.print (dimSizes[i]); 
                }
                System.out.print ("]\t");
                // if (numDims == 1) System.out.print ("\t");

                System.out.print((v.getRecVariance() ? "T" : "F")+"/");
                long[] dimVariances = v.getDimVariances();
                for (i=0; i < v.getNumDims(); i++)
                    System.out.print(
                        ((dimVariances[i] == CDFConstants.VARY) ? "T" : "F"));

                String sparseRecord = CDFUtils.getStringSparseRecord(v);
                if (!sparseRecord.equals("None"))
                     System.out.print (" sparseRecord=\""+sparseRecord+"\"");

                // Print the user-define pad value if one exists. 
		if (v.checkPadValueExistence()) {
                  padValue = v.getPadValue(); 
                  if (dt == CDF_EPOCH)
                    padValue = Epoch.encode(((Double) padValue).doubleValue());
                  else if (dt == CDF_EPOCH16)
                    padValue = Epoch16.encode((double[]) padValue);

                  System.out.print (" PadValue=\""+padValue+"\"");
                }

                blockingFactor = v.getBlockingFactor();
                if (blockingFactor > 0)
                    System.out.print (" blockingFactor=\""+blockingFactor+"\"");

                System.out.println (" ");
            }

            /**********************************************************/
            /* Print out the Variable data (all variables in the CDF) */ 
            /**********************************************************/
            System.out.println("\n\nVariable Data ("+n+" variables)\n"+
                               "=========================================");
            CDFData data = null;
            long   numRecs, maxRec;
            long[] dimIndices   = {0L};
            long[] dimIntervals = {1L};
            long[] dimSizes     = {1L};

            for (Enumeration e = vars.elements() ; e.hasMoreElements() ;) {
                Variable v = (Variable) e.nextElement();

                if (v.getNumDims() > 0) {
                    dimSizes = v.getDimSizes();
                    dimIntervals = new long[dimSizes.length];
                    dimIndices   = new long[dimSizes.length];
                    for (i=0; i < dimSizes.length; i++) {
                        dimIntervals[i] = 1;
                        dimIndices[i]   = 0;
                    }
                }
                maxRec = v.getMaxWrittenRecord();
                numRecs = v.getNumWrittenRecords();
                varName = v.getName();
                System.out.println (varName);
                for (i=0; i < varName.length(); i++)
                     System.out.print ("-");
                System.out.println (" ");

                if (v.getCompressionType() == NO_COMPRESSION)
                    System.out.println ("Compression:      None");
                else
                    System.out.println ("Compression:      "+
                                        v.getCompression()+" ("+
                                        v.getCompressionPct()+"%)");
		if (v.checkPadValueExistence()) {
		  long myDT = v.getDataType();
                  Object pv = v.getPadValue();
                  if (pv != null) {
		    if (myDT == CDF_EPOCH)
		      System.out.println ("Pad value:        "+
                         Epoch.encode(((Double)pv).doubleValue()));
		    else if (myDT == CDF_EPOCH16)
		      System.out.println ("Pad value:        "+
                         Epoch16.encode((double[])pv));
                    else
		      System.out.println ("Pad value:        "+ pv);
		  }
		}
                System.out.println ("Records:          "+
                                    numRecs+"n/"+maxRec+"x");
                System.out.println ("Allocated:        "+
                                    v.getNumAllocatedRecords()+"n/"+
                                    v.getMaxAllocatedRecord()+"x");
                System.out.println ("Blocking Factor:  "+
                                    v.getBlockingFactor());
                System.out.println ("Sparseness:       "+
                                    CDFUtils.getStringSparseRecord(v));
                System.out.println (" ");

                /********************************************************/
                /*  maxRec represents the last record number for this   */
                /*  variable, not the number of records.                */
                /*                                                      */
                /*  NOTE: maxRec starts at 0, so if the value of maxRec */
                /*        is 2, the actual number of records is 3.      */
                /*        If there are no records exists, the value of  */
                /*        maxRec is -1.                                 */
                /********************************************************/
                long status;
                for (i=0; i <= maxRec; i++) {
                     System.out.println ("Record # "+i+":");
                     data = v.getHyperDataObject(i, 1, 1,
                                                 dimIndices, 
                                                 dimSizes, 
                                                 dimIntervals);
                     status = cdf.getStatus();
                     if (status != VIRTUAL_RECORD_DATA) {
                         data.dumpData();
                         System.out.println(" ");                
                     }
                     else
                         System.out.println("    <virtual>\n");
                }
            }

            /**************************************************/
            /*  Print out a few individual variable data and  */
            /*  attribute entries.                            */
            /**************************************************/
            if (fileName.equals("test")) {
               System.out.println("\n\nIndividual Variable Data\n"+
                                   "=================================");

               Variable longitude = cdf.getVariable("Longitude"),   /* T/T */
                        latitude  = cdf.getVariable("Latitude"),    /* F/T */
                        latitude1 = cdf.getVariable("Latitude1"),   /* T/F */
                        time      = cdf.getVariable("Time"),        /* T/T */
                        foo       = cdf.getVariable("foo"),         /* F/F */
                        v         = cdf.getVariable("longitude_dup");

                System.out.println ("longDup.dumpData():");
                System.out.println ("------------------");
                if (v.getNumDims() > 0) {
                    dimSizes     = v.getDimSizes();
                    dimIntervals = new long[dimSizes.length];
                    dimIndices   = new long[dimSizes.length];
                    for (i=0; i < dimSizes.length; i++) {
                        dimIntervals[i] = 1;
                        dimIndices[i]   = 0;
                    }
                }
                maxRec = v.getMaxWrittenRecord();
                for (i=0; i <= maxRec; i++) {
                     System.out.println ("Record # "+i+":");
                     data = v.getHyperDataObject(i, 1, 1,
                                                 dimIndices,
                                                 dimSizes,
                                                 dimIntervals);
                     data.dumpData();
                     System.out.println(" ");
                }
 
               
               System.out.print ("Record #0 for latitude: ");
               CDFUtils.printData (latitude.getRecord (0));
               System.out.println ("");

               System.out.print ("Record #1 for longitude: ");
               CDFUtils.printData (longitude.getRecord (1));
               System.out.println ("");

               System.out.print ("Record #2 for latitude1: ");
               CDFUtils.printData (latitude1.getRecord (2));
               System.out.println ("");

               System.out.print ("Record #0 for foo: ");
               CDFUtils.printData (foo.getRecord (0));
               System.out.println ("\n");

               System.out.print ("1st element of record #0 for latitude: ");
               CDFUtils.printData (latitude.getSingleData(0, new long [] {0}));
               System.out.println ("");

               System.out.print ("2nd element of record #1 for longitude: ");
               CDFUtils.printData (longitude.getSingleData(1, new long [] {1}));
               System.out.println ("");

               System.out.print ("3rd element of record #2 for longitude: ");
               CDFUtils.printData (latitude1.getSingleData(2, new long [] {2}));
               System.out.println ("");

               System.out.print ("1st element of record #0 for foo: ");
               CDFUtils.printData (foo.getSingleData (0, new long [] {0}));
               System.out.println ("\n");

               CDFData dataRecord;
               System.out.print ("(1,0)th element of record #0 for Time: ");
               dataRecord = time.getSingleDataObject(0, new long[] {1,0});
               dataRecord.dumpData();

               System.out.print ("(1,1)th element of record #0 for Time: ");
               Long tValue = (Long) time.getSingleData(0, new long[] {1,1}); 
               System.out.println (tValue);

               System.out.println ("Record #0 for Time: ");
               dataRecord = time.getRecordObject(0L); 
               dataRecord.dumpData();
               System.out.println ("\n");

               System.out.println ("Record #0 for Time: ");
               long[][]  yy = (long [][]) time.getRecord(0L); 
               for (int x=0;x<3; x++)
                 for (int j=0;j<2;j++)
                      System.out.println("["+x+","+j+"] ="+yy[x][j]);

               System.out.println ("\n");

               Variable var;
               var = cdf.getVariable("Temperature2");
               System.out.print ("getScalarData for Temperature2: ");
               CDFUtils.printData (var.getScalarData());
               System.out.println ("\n");

               System.out.print ("getScalarDataObject for Temperature2: ");
               dataRecord = var.getScalarDataObject();
               dataRecord.dumpData();
               System.out.println ("");

               var = cdf.getVariable("Temperature");
               System.out.print ("Record #0 for Temperature: ");
               CDFUtils.printData (var.getScalarData(0L));
               System.out.println ("");
               System.out.print ("Record #1 for Temperature: ");
               dataRecord = var.getScalarDataObject(1L);
               dataRecord.dumpData();
               System.out.println ("\n");

               var = cdf.getVariable("Delta");
               System.out.println ("HyperGet for Delta: ");

               /***************************************************/
               /*  Read 3 records, starting from record number 0. */
               /*  Each record is an 3x2 array.                   */
               /***************************************************/
               int[][][] xxx = (int [][][]) var.getHyperData (0L, 3L, 1L, 
                                                             new long[] {0, 0},
                                                             new long[] {3, 2},
                                                             new long[] {1, 1});
               for (int x=0;x<3; x++) {
                 System.out.println ("\nRecord #"+x+":");
                 for (int j=0;j<3;j++) {
                   for (int k=0;k<2;k++) 
                        System.out.println("["+j+","+k+"] ="+xxx[x][j][k]);
                 }
               }

               /******************************************************/
               /*  Read the 2nd and 3rd row of the 3x2 array in the  */
               /*  1st and the 3rd record.                           */
               /******************************************************/
               int[][][] xx = (int [][][]) var.getHyperData (0L, 3L, 2L, 
                                                             new long[] {1, 0},
                                                             new long[] {2, 2},
                                                             new long[] {1, 1});
               for (int x=0;x<2; x++) {
                 int recNo = x;
                 if (x == 1) recNo = x+1;
                 System.out.println ("\nRecord #"+recNo+
                                     " (2nd and 3rd row of the 3x2 array):");
                 for (int j=0;j<2;j++) {
                   for (int k=0;k<2;k++) 
                        System.out.println("["+(j+1)+","+k+"] ="+xx[x][j][k]);
                 }
               }

               /*************************************************************/
               /* Initiate a record read for a group of CDF variables.      */ 
               /* Five (5) variables are involved in this data get function */
               /* and the 6th record for each variable is acquired.         */
               /*************************************************************/

	       /* record, the Vector holding the retrieved record objects */
	       Vector record;

	       String[] strVars = {"Longitude1", // variable names in CDF
			           "Temp1",
			           "dp",
				   "Temp",
				   "Temperature"};

	       long[] status = new long[6];
	       record = cdf.getRecord(5L, strVars, status);
	       /* record = cdf.getRecord(5L, varIDs, status);    */

	       System.out.println ("\nDump of the 6th record (Longitude1, "+
                                   "Temp1, dp, Temp, Temperature):");

               /* var: Longitude1 -- CDF_UINT2, dimensionality: 1:[3] */
	       System.out.print ("    Longitude1 -- "); 
	       for (i = 0; i < 3; i++) 
	         System.out.print (((int[])record.elementAt(0))[i]+" ");
	       System.out.println ("");

               /* var: Temp1 -- data type: CDF_REAL4 dimensionality: 1:[3] */
               System.out.print ("    Temp1 -- "); 
               for (i = 0; i < 3; i++) 
	         System.out.print (((float[])record.elementAt(1))[i]+" ");
               System.out.println ("");

               /* var: dp -- data type: CDF_DOUBLE dimensionality: 1:[3] */
               System.out.print ("    dp -- "); 
               for (i = 0; i < 3; i++) 
	         System.out.print (((double[])record.elementAt(2))[i]+" ");
               System.out.println ("");

               /* var: Temp -- data type: CDF_FLOAT dimensionality: 1:[3] */
               System.out.print ("    Temp -- "); 
               for (i = 0; i < 3; i++) 
	         System.out.print (((float[])record.elementAt(3))[i]+" ");
               System.out.println ("");

	       /* var: Temperature -- CDF_FLOAT, dimensionality: 0:[] */
               System.out.print ("    Temperature -- "); 
               System.out.print (record.elementAt(4)+" ");
               System.out.println ("");

               /******************************************************/
               /*  Read the 1st column of the 3x2 array in the       */
               /*  1st and the 2nd record.                           */
               /******************************************************/
               int[][] xx1 = (int [][]) var.getHyperData (0L, 2L, 1L, 
                                                          new long[] {0, 0},
                                                          new long[] {3, 1},
                                                          new long[] {1, 1});
               for (int x=0; x < 2; x++) {
                 int recNo = x;
                 if (x == 1) recNo = 1;
                 System.out.println ("\nRecord #"+recNo+
                                     " (1st column of the 3x2 array):");
                 for (int j=0; j < 3; j++) 
                    System.out.println("["+j+",0] ="+xx1[x][j]);
               }
               System.out.println ("\n");
                                 

               System.out.println("\n\nVariable/Global Attribute Entries"+
                                  "\n====================================");

               Attribute  test = cdf.getAttribute("Test"),         /* global */
                          validMin = cdf.getAttribute("validmin"); /* var */

               Entry  tEntry = test.getEntry(15),
                      vEntry = validMin.getEntry(longitude); 

               long attrNum = test.getID();
               Attribute test1 = cdf.getAttribute(attrNum);

               System.out.print (test1.getName()+": \n\t");
               CDFUtils.printData (tEntry.getData());
               System.out.println ("");

               System.out.print ("\nVALIDMIN: \n\tLongitude: ");
               CDFUtils.printData (vEntry.getData());
               System.out.println ("");

               System.out.print ("\nVALIDMAX: \n\tLatitude: ");
               var = cdf.getVariable("Latitude");
               CDFUtils.printData (var.getEntryData("VALIDMAX"));
               System.out.println ("");

            }

            cdf.close();

        } catch (Exception e) {
             System.out.println (e);
        }
    }
}
