�MKMSGF GEN+ �               u � -��$g�O���K���E	4	�	��Jv������q��C^u:You must enter a value for each of the five command parameters.
:The host operating system you specified is not valid.
         Valid values are "MVS" or VM".
:Unable to open %2.
:
An <option> you specified is not valid.
Valid options are: u, d, a.
The combination of <options> you enter must be preceded by a /.
:
A <destination> you specified is not valid.
:File transfer error (Communications Manager return code=%3).
:
Insert a diskette into drive %1
and press <enter> to continue.
:
Insert '%2' into drive %1
and press <enter> to continue.
:Loading diskette '%2' failed (return code=%1).
         Select Y to retry or N to cancel the program.
:File %1 could not be placed on the diskette because it is full.
:An error occurred while compressing '%3'. (return code=%2).
:DISKGEN is unable to determine the available space on the
         diskette. Select Y to retry or N to cancel the program.
:DISKGEN is unable to determine the size of %1.
         Check to be sure the file exists.
         Select Y to retry or N to cancel the program.
:Not enough DISK entries were specified in the input
         package file. Add more DISK entries and start DISKGEN again.
:An error occurred while labelling the diskette.
         Insert another diskette and press <enter> to continue.
:File '%1' has a size of 0.
         The component and product size calculations of the output
         package file will be inaccurate.
:The default compression program is EPFIPAK2.
:
Select Y if you want to use EPFIPAK2.  Select N if you want
to use any other compression program.
:An input package file name must be specified.
:
If you do not specify a new path and name for the output
package file, it will have the same name as the input package
file with an extension of .GEN. The output package file will
be put in the same directory as the input package file.
:
Type a new name for the output package file or press <enter>
to use the input package file name and continue.
:
Enter the name of the compression program you want to use.
Include all of the parameters that you want the compression
program to use. Use the extensions .INP and .OUT to 
designate input and output files.
:Press enter to continue.
:Syntax: DISKGEN </options> <system> <input package file>
<destination> <source_directory>
:</options>: Must be prefaced with a / and can be in any order.
         For example, /ua.
         a    Appends the directory specified in the PWS keywords
              of FILE entries to the <source_directory> when searching
              for files.
         d    Deletes the source files from the <source_directory> after
              transfer.
         u    Updates the package file and produces an output package file
              in the <source_directory>.

<system>: Valid values are "MVS", "VM", or "DRIVE".
:
<input package file>: The PWS file name of the package file to
        be updated or used for the sending of files.

<destination>: If /d or /ud options are specified, 
         <destination> is the host session or drive location to 
         transfer the packed files to.
         The format is:
           MVS:   sessionid:datasetname  (b:pdfttst3.decandio)
           VM:    sessionid:mode         (b:a)
           DRIVE: drive:                 (a:)

<source_directory>: The workstation directory where the files
         specified in the input package file are located. 
:Parameters must be specified with DISKGEN.
:Calculated size of the entire product: %1
:Calculated sum of all component sizes: %1
:Packing:
:Packing of file failed. Retry (Y or N)?
:Copying:  %1
:To:   %1
:Uploading:  %1
:Send Failed. Retry (Y or N)?
:Transfer Complete.
If you used an alternate packing program you need to
change your UNPACK keyword in the package file.
:Transfer Cancelled.
:You have specified a drive that is not valid. Valid values are A: or B:.
:DISKGEN will be executed with the following parameters.
:Select Y to continue or N to type the command again.
:Re-Enter the parameters.
:Press enter to exit.
:The file %2 cannot be copied to the diskette because it is too large. The file size is %1.
