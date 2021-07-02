package queries;

import actor.ActorsAwards;
import fileio.ActionInputData;
import fileio.ActorInputData;
import fileio.Writer;
import newfileio.Input;
import org.json.simple.JSONObject;
import solution.NameNumber;
import solution.SortAscNo;
import solution.SortDescNo;
import utils.Utils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * The class contains method that solves a query
 * of type "Actors" with criteria "Awards"
 */
public class ActorAwardsQuery {

    private final Input input;
    private final Writer fileWriter;

    public ActorAwardsQuery(final Input input, final Writer fileWriter) {
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
        final List<String> awards = query.getFilters().get(3);
        List<NameNumber> actorsAwards = new ArrayList<>();

        for (ActorInputData actor : actors) {
            Map<ActorsAwards, Integer> awardsNo = actor.getAwards();

            // in awards we have the filter
            int i;
            for (i = 0; i < awards.size(); i++) {
                ActorsAwards awardName = Utils.stringToAwards(awards.get(i));
                // if no awardName in list of awards of the actors
                if (!awardsNo.containsKey(awardName)) {
                    break;
                }
            }
            if ((awards.size() == 0) || (i != awards.size())) {
                continue;
            }
            // if reach here awards were found
            // now let's see how many awards each one has
            int noAwards = 0;
            for (Map.Entry<ActorsAwards, Integer> crtAward : awardsNo.entrySet()) {
                noAwards += crtAward.getValue();
            }
            actorsAwards.add(new NameNumber(actor.getName(), (double) noAwards));
        }
        if (query.getSortType().equals("desc")) {
            actorsAwards.sort(new SortDescNo());
        } else {
            actorsAwards.sort(new SortAscNo());
        }
        List<String> finalActors = new ArrayList<>();
        int i = 0;
        while (i < actorsAwards.size()) {
            finalActors.add(actorsAwards.get(i).getName());
            i++;
        }
        String message = "";
        message += "Query result: ";
        message += finalActors.toString();
        return fileWriter.writeFile(query.getActionId(), "", message);
    }
}
