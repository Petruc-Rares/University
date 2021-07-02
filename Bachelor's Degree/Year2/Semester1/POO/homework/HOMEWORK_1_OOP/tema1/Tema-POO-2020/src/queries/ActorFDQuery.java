package queries;

import fileio.ActionInputData;
import fileio.ActorInputData;
import fileio.Writer;
import newfileio.Input;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

/**
 * The class contains method that solves a query
 * of type "Actors" with criteria "filter_description"
 */
public class ActorFDQuery {

    private final Input input;
    private final Writer fileWriter;

    public ActorFDQuery(final Input input, final Writer fileWriter) {
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current query
     * @param query represents the current query
     * @return JSONObject in the desired format
     * @throws IOException in case of exceptions to reading / writing
     */
    public JSONObject solveQuery(final ActionInputData query) throws IOException {
        List<ActorInputData> actors = input.getActors();
        final List<String> words = query.getFilters().get(2);
        List<String> finalActors = new ArrayList<>();
        int wordsSize = words.size();

        for (ActorInputData actor : actors) {
            String careerDescription = " ";
            // COMPOSE WORDS
            String newCareerDescription =
                    actor.getCareerDescription().replaceAll("[-]", " ");
            careerDescription += newCareerDescription
                    .replaceAll("[^a-zA-Z0-9 ]", "")
                    .toLowerCase();
            careerDescription += " ";

            int i;
            for (i = 0; i < wordsSize; i++) {
                String searchedWord = " ";
                searchedWord += words.get(i);
                searchedWord += " ";
                // if it doesn't contain break
                if (!careerDescription.contains(searchedWord)) {
                    break;
                }
            }
            if (i == wordsSize) {
                finalActors.add(actor.getName());
            }
        }
        if (query.getSortType().equals("asc")) {
            finalActors.sort(Comparator.comparing(String::toString));
        } else {
            finalActors.sort(Comparator.comparing(String::toString).reversed());
        }
        String message = "";
        message += "Query result: ";
        message += finalActors.toString();
        return fileWriter.writeFile(query.getActionId(), "", message);
    }

}
