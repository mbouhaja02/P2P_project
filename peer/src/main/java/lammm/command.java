package lammm;

import java.io.PrintWriter;
import java.util.List;


public abstract class command{
    private String name;

    public command(String name){
        this.name = name;
    }

    public String getName(){
        return this.name;
    }
    public abstract boolean isValid(String input); // Checks if the input follows the right format
    public abstract void execute(String input, List<FileWrapper> files, PrintWriter out); // executes the command 
}