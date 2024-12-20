package lammm;

import java.io.*;
import java.nio.file.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;


public class FileManager {
    // private static final String SEED_DIR = "./seed";
    private String SEED_DIR; // depends on the peer id

    public FileManager(String DirectoryPath) {
        this.SEED_DIR = DirectoryPath;
    }

    public String getSeedFilesList() throws IOException {
        try (Stream<Path> paths = Files.walk(Paths.get(this.SEED_DIR))) {
            return paths
                    .filter(Files::isRegularFile)
                    .map(path -> new FileWrapper(path.getFileName().toString(), path.toString()).formatFileInfo())
                    .collect(Collectors.joining(" ", "[", "]"));
        }
    }

}
