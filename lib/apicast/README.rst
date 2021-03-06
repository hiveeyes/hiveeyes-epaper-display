############################################
Apicast HTTP to JSON data export for Arduino
############################################


*****
About
*****

This code fetches data from the *Hiveeyes Apicast* service.

It converts data from the bee flight forecast information published by the
German Weather Service (DWD) into JSON format.

Manny thanks to the DWD and Andreas Motl.


********
Synopsis
********

Example of the data endpoint::

    http://apicast.hiveeyes.org/beeflight/forecast/germany/brandenburg/potsdam

The list of possible location slugs can be obtained from:

    http://apicast.hiveeyes.org/beeflight/stations/germany/locations

The available locations as of 2021 are::

    [
      "baden-wurttemberg/freudenstadt",
      "baden-wurttemberg/konstanz",
      "baden-wurttemberg/lahr",
      "baden-wurttemberg/mannheim",
      "baden-wurttemberg/ohringen",
      "baden-wurttemberg/rheinstetten",
      "baden-wurttemberg/stotten",
      "baden-wurttemberg/stuttgart-flughafen",
      "bayern/augsburg",
      "bayern/bamberg",
      "bayern/furstenzell",
      "bayern/hof",
      "bayern/hohenpeissenberg",
      "bayern/kempten",
      "bayern/munchen-flughafen",
      "bayern/nurnberg-flughafen",
      "bayern/oberstdorf",
      "bayern/regensburg",
      "bayern/straubing",
      "bayern/weiden",
      "bayern/wurzburg",
      "berlin/berlin-tempelhof",
      "brandenburg/angermunde",
      "brandenburg/cottbus",
      "brandenburg/lindenberg",
      "brandenburg/potsdam",
      "bremen/bremen-flughafen",
      "hamburg/hamburg-flughafen",
      "hessen/frankfurt-flughafen",
      "hessen/fritzlar",
      "hessen/offenbach-wetterpark",
      "hessen/wasserkuppe",
      "hessen/wettenberg-bei-giessen",
      "mecklenburg-vorpommern/arkona",
      "mecklenburg-vorpommern/greifswald",
      "mecklenburg-vorpommern/marnitz",
      "mecklenburg-vorpommern/rostock-warnemunde",
      "mecklenburg-vorpommern/schwerin",
      "mecklenburg-vorpommern/waren-muritz",
      "niedersachsen/cuxhaven",
      "niedersachsen/emden",
      "niedersachsen/hannover-flughafen",
      "niedersachsen/luchow",
      "niedersachsen/norderney",
      "nordrhein-westfalen/aachen-orsbach",
      "nordrhein-westfalen/bad-lippspringe",
      "nordrhein-westfalen/dusseldorf-flughafen",
      "nordrhein-westfalen/essen-bredeney",
      "nordrhein-westfalen/kahler-asten",
      "nordrhein-westfalen/koln-bonn-flughafen",
      "nordrhein-westfalen/munster-osnabruck-flughafen",
      "rheinland-pfalz/hahn",
      "rheinland-pfalz/nurburg-barweiler",
      "rheinland-pfalz/trier-petrisberg",
      "saarland/saarbrucken-flughafen",
      "sachsen-anhalt/magdeburg",
      "sachsen/dresden-klotzsche-flughafen",
      "sachsen/gorlitz",
      "sachsen/leipzig-halle-flughafen",
      "schleswig-holstein/fehmarn",
      "schleswig-holstein/helgoland",
      "schleswig-holstein/kiel-holtenau",
      "schleswig-holstein/list-auf-sylt",
      "schleswig-holstein/schleswig",
      "thuringen/erfurt-weimar-flughafen",
      "thuringen/gera-leumnitz",
      "thuringen/meiningen"
    ]
