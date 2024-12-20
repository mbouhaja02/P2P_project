package lammm;

import java.io.PrintWriter;
import java.util.List;
import java.util.regex.*;

public class getpieces extends command {
    public getpieces(String name) {
        super(name);
    }

    public boolean isValid(String input) {
        String[] parsedInput = input.split(" ");
        int startIndex = input.indexOf("[");
        int endIndex = input.indexOf("]");
        if (parsedInput.length > 1) {
            if (parsedInput[0].equals("getpieces") && parsedInput[1].length() == 32 && startIndex > -1
                    && endIndex > -1) {
                String pattern = "\\[\\d+( \\d+)*\\]";
                String list = input.substring(startIndex, endIndex + 1);
                Pattern regex = Pattern.compile(pattern);
                Matcher matcher = regex.matcher(list);
                return matcher.matches();
            }
        }
        return false;
    }

    public void execute(String input, List<FileWrapper> files, PrintWriter out) {
        String[] parsedInput = input.split(" ");
        // check wether it's valid
        if (this.isValid(input)) {
            // extract arguments
            String filekey = parsedInput[1];
            Pattern pattern = Pattern.compile("\\[(.*?)\\]");
            Matcher matcher = pattern.matcher(input);

            String pieces = "";
            if (matcher.find()) {
                String list = matcher.group(1);
                String[] numbers = list.split("\\s+");
                StringBuilder result = new StringBuilder();
                for (String num : numbers) {
                    result.append(num);
                }
                pieces = result.toString();
            }
            out.print("data " + filekey + " [");
            // Cherche BufferMap
            for (FileWrapper file : files) {
                if (file.getKey().equals(filekey)) {
                    for (char piece : pieces.toCharArray()) {
                        System.out.println(piece);
                        out.print(piece + ":%");
                        file.readPeace(out, Integer.parseInt("" + piece));
                        out.print("% ");
                        // out.flush();
                    }
                }
            }
            out.println("]");
        }
    }
}