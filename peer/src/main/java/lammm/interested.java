package lammm;

import java.io.PrintWriter;
import java.util.List;

public class interested extends command {
    public interested(String name) {
        super(name);
    }

    public boolean isValid(String input) {
        String[] parsedInput = input.split(" ");
        return parsedInput.length == 2 && parsedInput[0].equals("interested") && parsedInput[1].length() == 32;
    }

    public void execute(String input, List<FileWrapper> files, PrintWriter out) {
        String[] parsedInput = input.split(" ");

        // check wether it's valid
        if (this.isValid(input)) {
            // extract arguments

            String filekey = parsedInput[1];
            // for (FileWrapper file : files) {
            System.out.println(files.size());
            for (int i = 0; i < files.size(); i++) {
                if (files.get(i).getKey().equals(filekey)) {
                    System.out.println("have " + filekey + " " +
                            files.get(i).getBufferMapString());
                    out.println("have " + filekey + " " + files.get(i).getBufferMapString());
                    out.flush();
                    break;
                }

            }
        }
    }
}