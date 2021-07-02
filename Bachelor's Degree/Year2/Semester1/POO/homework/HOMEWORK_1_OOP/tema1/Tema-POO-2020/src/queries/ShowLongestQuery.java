package queries;

import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import newfileio.SerialData;
import org.json.simple.JSONObject;
import solution.NameNumber;
import utils.Utils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class ShowLongestQuery {

    private final Input input;
    private final Writer fileWriter;

    public ShowLongestQuery(final Input input, final Writer fileWriter) {
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
        String desiredYear = query.getFilters().get(0).get(0);
        String desiredGenre = query.getFilters().get(1).get(0);

        List<SerialData> serials = input.getSerials();
        List<NameNumber> filmsData = new ArrayList<>();
        for (SerialData serial: serials) {
            Utils.filterFilmsDuration(null, serial, filmsData,
                    desiredYear, desiredGenre);
        }
        return fileWriter.writeFile(query.getActionId(), "",
                Utils.buildFilms(query, filmsData));
    }
}
