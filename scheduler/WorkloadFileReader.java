/*
 * Title:        CloudSim Toolkit
 * Description:  CloudSim (Cloud Simulation) Toolkit for Modeling and Simulation of Clouds
 * Licence:      GPL - http://www.gnu.org/copyleft/gpl.html
 *
 * Copyright (c) 2009-2012, The University of Melbourne, Australia
 */

package org.cloudbus.cloudsim.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.zip.GZIPInputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import org.cloudbus.cloudsim.Cloudlet;
import org.cloudbus.cloudsim.UtilizationModel;
import org.cloudbus.cloudsim.UtilizationModelFull;

/**
 * This class is responsible for reading resource traces from a file and creating a list of jobs.
 * <p>
 * <b>NOTE:</b>
 * <ul>
 * <li>This class can only take <tt>one</tt> trace file of the following format: <i>ASCII text, zip,
 * gz.</i>
 * <li>If you need to load multiple trace files, then you need to create multiple instances of this
 * class <tt>each with a unique 
 *      	 entity name</tt>.
 * <li>If size of the trace file is huge or contains lots of traces please increase the JVM heap
 * size accordingly by using <tt>java -Xmx</tt> option when running the simulation.
 * <li>The default job file size for sending to and receiving from a resource is
 * {@link gridsim.net.Link#DEFAULT_MTU}. However, you can specify the file size by using
 * {@link #setGridletFileSize(int)}.
 * <li>A job run time is only for 1 PE <tt>not</tt> the total number of allocated PEs. Therefore, a
 * Gridlet length is also calculated for 1 PE.<br>
 * For example, job #1 in the trace has a run time of 100 seconds for 2 processors. This means each
 * processor runs job #1 for 100 seconds, if the processors have the same specification.
 * </ul>
 * <p>
 * By default, this class follows the standard workload format as specified in <a
 * href="http://www.cs.huji.ac.il/labs/parallel/workload/">
 * http://www.cs.huji.ac.il/labs/parallel/workload/</a> <br>
 * However, you can use other format by calling the below methods before running the simulation:
 * <ul>
 * <li> {@link #setComment(String)}
 * <li> {@link #setField(int, int, int, int, int)}
 * </ul>
 * 
 * @author Anthony Sulistio and Marcos Dias de Assuncao
 * @since 5.0
 * 
 * @see Workload
 */
public class WorkloadFileReader implements WorkloadModel {

	private final File file; // file name

	private final int rating; // a PE rating

	private ArrayList<Cloudlet> jobs = null; // a list for getting all the

	/*
	 * #define JOB_ID                  0
  7 #define JOB_ARRIVAL     1
  8 #define JOB_SCHEDTIME   2
  9 #define JOB_FINISH      3
 10 #define JOB_SCHEDCLASS  4
 11 #define JOB_TASKS               5
 12 #define JOB_CPU                 6
 13 #define JOB_MEM                 7
 14 #define JOB_USER                8
 15 #define JOB_SLA1                9
 16 #define JOB_SLA2                10
 17 #define JOB_SLA3                11
 6251639646,4046279,5569012,75251710,1,1,100,1060,1,0,0,0

	 */
	// Gridlets

	// using Standard Workload Format
	//private long JOB_NUM = 1 - 1; // job number
	  private int JOB_ID = 1 - 1; // job id

	//private int SUBMIT_TIME = 2 - 1; // submit time of a Gridlet
	  private int JOB_ARRIVAL = 2 - 1; // arrival time of a Gridlet
	  private int JOB_SCHEDTIME = 3 - 1; // running time of a Gridlet
	  private int JOB_FINISH = 4 - 1; // finish time of a Gridlet
	  
	  private int JOB_SCHEDCLASS = 5 - 1; // scheduling class of Job
	  
	  private int JOB_TASKS = 6 - 1;
	  
	  private int JOB_CPU = 7 - 1;
	  private int JOB_MEM = 8 - 1;
	  
	  private int JOB_USER = 9 - 1;
	  
	  private int JOB_SLA1 = 10 - 1;

	  private int JOB_SLA2 = 11 - 1;

	  private int JOB_SLA3 = 12 - 1;
	/*  
	  //private final int RUN_TIME = 4 - 1; // running time of a Gridlet

	private final int NUM_PROC = 5 - 1; // number of processors needed for a

	// Gridlet
	private int REQ_NUM_PROC = 8 - 1; // required number of processors

	private int REQ_RUN_TIME = 9 - 1; // required running time

	private final int USER_ID = 12 - 1; // if of user who submitted the job

	private final int GROUP_ID = 13 - 1; // if of group of the user who
*/
	// submitted the
	// job
     private int MAX_FIELD = 12; // max number of field in the trace file

	 private String COMMENT = ";"; // a string that denotes the start of a

	// comment
	private static final int IRRELEVANT = -1; // irrelevant number

	private String[] fieldArray = null; // a temp array storing all the fields

	/**
	 * Create a new {@link WorkloadFileReader} object.
	 * 
	 * @param fileName the workload trace filename in one of the following format: <i>ASCII text,
	 *        zip, gz.</i>
	 * @param rating the resource's PE rating
	 * @throws FileNotFoundException
	 * @throws IllegalArgumentException This happens for the following conditions:
	 *         <ul>
	 *         <li>the workload trace file name is null or empty
	 *         <li>the resource PE rating <= 0
	 *         </ul>
	 * @pre fileName != null
	 * @pre rating > 0
	 * @post $none
	 */
	public WorkloadFileReader(final String fileName, final int rating) throws FileNotFoundException {
		if (fileName == null || fileName.length() == 0) {
			throw new IllegalArgumentException("Invalid trace file name.");
		} else if (rating <= 0) {
			throw new IllegalArgumentException("Resource PE rating must be > 0.");
		}

		file = new File(fileName);
		if (!file.exists()) {
			throw new FileNotFoundException("Workload trace " + fileName + " does not exist");
		}
       // System.out.println("File Checked");
		this.rating = rating;
	}

	/**
	 * Reads job information from a given file.
	 * 
	 * @return the list of gridlets read from the file; <code>null</code> in case of failure.
	 */
	@Override
	public ArrayList<Cloudlet> generateWorkload() {
		if (jobs == null) {
			jobs = new ArrayList<Cloudlet>();

			// create a temp array
			fieldArray = new String[MAX_FIELD];

			try {
				if (file.getName().endsWith(".gz")) {
					readGZIPFile(file);
				} else if (file.getName().endsWith(".zip")) {
					readZipFile(file);
				} else {
					readFile(file);
				}
			} catch (final FileNotFoundException e) {
			} catch (final IOException e) {
			}
		}
		// System.out.println("Genearting Workload");
		return jobs;
	}

	/**
	 * Identifies the start of a comment line.
	 * 
	 * @param cmt a character that denotes the start of a comment, e.g. ";" or "#"
	 * @return <code>true</code> if it is successful, <code>false</code> otherwise
	 * @pre comment != null
	 * @post $none
	 */
	public boolean setComment(final String cmt) {
		boolean success = false;
		if (cmt != null && cmt.length() > 0) {
			COMMENT = cmt;
			success = true;
		}
		//System.out.println("Setting Comment");
		return success;
	}

	/**
	 * Tells this class what to look in the trace file. This method should be called before the
	 * start of the simulation.
	 * <p>
	 * By default, this class follows the standard workload format as specified in <a
	 * href="http://www.cs.huji.ac.il/labs/parallel/workload/">
	 * http://www.cs.huji.ac.il/labs/parallel/workload/</a> <br>
	 * However, you can use other format by calling this method.
	 * <p>
	 * The parameters must be a positive integer number starting from 1. A special case is where
	 * <tt>jobNum == -1</tt>, meaning the job or gridlet ID starts at 1.
	 * 
	 * @param maxField max. number of field/column in one row
	 * @param jobNum field/column number for locating the job ID
	 * @param submitTime field/column number for locating the job submit time
	 * @param runTime field/column number for locating the job run time
	 * @param numProc field/column number for locating the number of PEs required to run a job
	 * @return <code>true</code> if successful, <code>false</code> otherwise
	 * @throws IllegalArgumentException if any of the arguments are not within the acceptable ranges
	 * @pre maxField > 0
	 * @pre submitTime > 0
	 * @pre runTime > 0
	 * @pre numProc > 0
	 * @post $none
	 */
	/*
	 * #define JOB_ID                  0
  7 #define JOB_ARRIVAL     1
  8 #define JOB_SCHEDTIME   2
  9 #define JOB_FINISH      3
 10 #define JOB_SCHEDCLASS  4
 11 #define JOB_TASKS               5
 12 #define JOB_CPU                 6
 13 #define JOB_MEM                 7
 14 #define JOB_USER                8
 15 #define JOB_SLA1                9
 16 #define JOB_SLA2                10
 17 #define JOB_SLA3                11
 6251639646,4046279,5569012,75251710,1,1,100,1060,1,0,0,0*/
	
	public boolean setField(
			final int maxField,
			final int jobNum,
			final int schedTime,
			final int finishTime,
			final int numProc) {
		// need to subtract by 1 since array starts at 0.
		if (jobNum > 0) {
			JOB_ID = jobNum - 1;
		} else if (jobNum == 0) {
			throw new IllegalArgumentException("Invalid job number field.");
		} else {
			JOB_ID = -1;
		}

		// get the max. number of field
		if (maxField > 0) {
			MAX_FIELD = maxField;
		} else {
			throw new IllegalArgumentException("Invalid max. number of field.");
		}

		// get the submit time field
		if (schedTime > 0) {
			JOB_SCHEDTIME = schedTime - 1;
		} else {
			throw new IllegalArgumentException("Invalid submit time field.");
		}

		// get the run time field
		if (finishTime > 0) {
			JOB_FINISH = finishTime - 1;
		} else {
			throw new IllegalArgumentException("Invalid run time field.");
		}

		// get the number of processors field
		if (numProc > 0) {
			JOB_CPU = numProc - 1;
		} else {
			throw new IllegalArgumentException("Invalid number of processors field.");
		}
		//System.out.println("Set Field");
		return true;
		 
	}

	// ------------------- PRIVATE METHODS -------------------

	/**
	 * Creates a Gridlet with the given information and adds to the list
	 * 
	 * @param id a Gridlet ID
	 * @param submitTime Gridlet's submit time
	 * @param runTime Gridlet's run time
	 * @param numProc number of processors
	 * @param reqRunTime user estimated run time
	 * @param userID user id
	 * @param groupID user's group id
	 * @pre id >= 0
	 * @pre submitTime >= 0
	 * @pre runTime >= 0
	 * @pre numProc > 0
	 * @post $none
	 */
	private void createJob(
			final long id,
			final long arrivalTime,
			final long schedTime,
			final long finishTime,
			final int schedClass,
			final int tasks,
			final int cpu,
			final int mem,
			final int user) {
		// create the cloudlet
		final long runTime= finishTime-schedTime;
		final long len =  runTime * rating;
		UtilizationModel utilizationModel = new UtilizationModelFull();
		final Cloudlet wgl = new Cloudlet(
				(int)id,
				len,
				cpu,
				300,
				300,
				utilizationModel,
				utilizationModel,
				utilizationModel);
		
		jobs.add(wgl);
		System.out.println("Length"+ len );
	}

	/**
	 * Extracts relevant information from a given array
	 * 
	 * @param array an array of String
	 * @param line a line number
	 * @pre array != null
	 * @pre line > 0
	 */
	private void extractField(final String[] array, final int line) {
		try {
			long id=0;
			//System.out.println("extract");
			//System.out.println("JOB_ID" + JOB_ID);
			 Long obj=null;
			 			//int id = 0;
			if (JOB_ID == IRRELEVANT) {
			//	System.out.println("JOB_ID IRREL" + JOB_ID);
				id = jobs.size() + 1;
			} else {
				//System.out.println("array[JOB_ID]" + array[JOB_ID]);
				obj = new Long((array[JOB_ID]).trim());
				id = obj.intValue();

				//id = new Long(array[JOB_ID].trim()).intValue();
				
				//id = obj.intValue();
				//System.out.println("JOB_ID rel" + id);
			}
			//System.out.println("Extract Field 0");
			// get the submit time
			final Long l = new Long(array[JOB_ARRIVAL].trim());
		    long arrivalTime = l.intValue();

			// get the user estimated run time
		    Integer obj1=null;
			obj1 = new Integer(array[JOB_SCHEDTIME ].trim());
		    long schedTime = obj1.intValue();

			// if the required run time field is ignored, then use
			// the actual run time
			obj1 = new Integer(array[JOB_FINISH].trim());
			long finishTime = obj1.intValue();
			
			int schedClass = new Integer(array[JOB_SCHEDCLASS].trim()).intValue();

			int tasks = new Integer(array[JOB_TASKS].trim()).intValue();
 
			int cpu = new Integer(array[JOB_CPU].trim()).intValue();
			 
			int mem = new Integer(array[JOB_MEM].trim()).intValue();
			
			int user = new Integer(array[JOB_USER].trim()).intValue();
			
			int param1 = new Integer(array[JOB_SLA1].trim()).intValue();
			
			int param2 = new Integer(array[JOB_SLA2].trim()).intValue();
			
			int param3 = new Integer(array[JOB_SLA3].trim()).intValue();
			 
			//System.out.println("Extract Field");
			createJob(id, arrivalTime, schedTime, finishTime, schedClass, tasks, cpu, mem, user);
		} catch (final Exception e) {

		}
	}
	/*
	 * final int id,
			final long arrivalTime,
			final long schedTime,
			final long finishTime,
			final int schedClass,
			final int tasks,
			final int cpu,
			final int mem,
			final int user
	 */

	/**
	 * Breaks a line of string into many fields.
	 * 
	 * @param line a line of string
	 * @param lineNum a line number
	 * @pre line != null
	 * @pre lineNum > 0
	 * @post $none
	 */
	private void parseValue(final String line, final int lineNum) {
		// skip a comment line
		if (line.startsWith(COMMENT)) {
			return;
		}

		final String[] sp = line.split("\\s*,\\s*"); // split the fields based on a
		// space
		int len = 0; // length of a string
		int index = 0; // the index of an array

		// check for each field in the array
		for (final String elem : sp) {
			len = elem.length(); // get the length of a string

			// if it is empty then ignore
			if (len == 0) {
				continue;
			}
			fieldArray[index] = elem;
			index++;
		}
		//System.out.println("Parse Value");
		//System.out.println("Index="+ index +"Max Field = "+ MAX_FIELD);
		if (index == MAX_FIELD) {
			//System.out.println("Parse Value");
			extractField(fieldArray, lineNum);
		}
	}

	/**
	 * Reads a text file one line at the time
	 * 
	 * @param fl a file name
	 * @return <code>true</code> if successful, <code>false</code> otherwise.
	 * @throws IOException if the there was any error reading the file
	 * @throws FileNotFoundException if the file was not found
	 */
	private boolean readFile(final File fl) throws IOException, FileNotFoundException {
		boolean success = false;
		BufferedReader reader = null;
		try {
			reader = new BufferedReader(new InputStreamReader(new FileInputStream(fl)));

			// read one line at the time
			int line = 1;
			while (reader.ready()) {
				parseValue(reader.readLine(), line);
				line++;
			}

			reader.close();
			success = true;
		} finally {
			if (reader != null) {
				reader.close();
			}
		}
		//System.out.println("Reading file");
		return success;
	}

	/**
	 * Reads a gzip file one line at the time
	 * 
	 * @param fl a gzip file name
	 * @return <code>true</code> if successful; <code>false</code> otherwise.
	 * @throws IOException if the there was any error reading the file
	 * @throws FileNotFoundException if the file was not found
	 */
	private boolean readGZIPFile(final File fl) throws IOException, FileNotFoundException {
		boolean success = false;
		BufferedReader reader = null;
		try {
			reader = new BufferedReader(new InputStreamReader(new GZIPInputStream(new FileInputStream(fl))));

			// read one line at the time
			int line = 1;
			while (reader.ready()) {
				parseValue(reader.readLine(), line);
				line++;
			}

			reader.close();
			success = true;
		} finally {
			if (reader != null) {
				reader.close();
			}
		}
		System.out.println("read GZIP File");
		return success;
	}

	/**
	 * Reads a Zip file.
	 * 
	 * @param fl a zip file name
	 * @return <code>true</code> if reading a file is successful; <code>false</code> otherwise.
	 * @throws IOException if the there was any error reading the file
	 */
	private boolean readZipFile(final File fl) throws IOException {
		boolean success = false;
		ZipFile zipFile = null;
		try {
			BufferedReader reader = null;

			// ZipFile offers an Enumeration of all the files in the file
			zipFile = new ZipFile(fl);
			final Enumeration<? extends ZipEntry> e = zipFile.entries();
			while (e.hasMoreElements()) {
				success = false; // reset the value again
				final ZipEntry zipEntry = e.nextElement();

				reader = new BufferedReader(new InputStreamReader(zipFile.getInputStream(zipEntry)));

				// read one line at the time
				int line = 1;
				while (reader.ready()) {
					parseValue(reader.readLine(), line);
					line++;
				}

				reader.close();
				success = true;
			}
		} finally {
			if (zipFile != null) {
				zipFile.close();
			}
		}
		System.out.println("read Zip File");
		return success;
	}
}

