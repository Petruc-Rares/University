package fileio;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

/**
 * this class helps in writing some output to the specified outputFileName
 */
public class OutputWriter {
    private String outputFileName;

    public OutputWriter(String outputFileName) {
        this.outputFileName = outputFileName;
    }

    /**
     * writes to the outputFileName
     * @param text representing what needs to be written
     */
    public void writeOutput(String text) {
        BufferedWriter bw = null;
        try {
            //Specify the file name and path here
            File file = new File(outputFileName);

            /* This logic will make sure that the file
             * gets created if it is not present at the
             * specified location*/
            if (!file.exists()) {
                file.createNewFile();
            }

            FileWriter fw = new FileWriter(file);
            bw = new BufferedWriter(fw);
            bw.write(text);
        } catch (
                IOException ioe) {
            ioe.printStackTrace();
        } finally {
            try {
                if (bw != null)
                    bw.close();
            } catch (Exception ex) {
                System.out.println("Error in closing the BufferedWriter" + ex);
            }
        }
    }
}
