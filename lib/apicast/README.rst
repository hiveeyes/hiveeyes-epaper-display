#####################################
Apicast HTTP to Json data export for Arduino
#####################################


*****
About
*****
This code fetches data from the Apicast-Hiveeyes-Platform.
It converts data from the bee flight forecast of the German Weather Service (DWD)into the .Json format
Manny Thanks to Andreas Mot

Example of the API:
http://apicast.hiveeyes.org/beeflight/forecast/germany/berlin_brandenburg/potsdam

available locations for the forecast:

baden-wuerttemberg, Baden-Württemberg
  freudenstadt, Freudenstadt im Schwarzwald
  konstanz, Konstanz am Bodensee
  lahr, Lahr
  mannheim, Mannheim
  oehringen, Öhringen
  rheinstetten, Rheinstetten
  stoetten, Stötten (Geislingen an der Steige)
  stuttgart, Stuttgart (Flughafen)
bayern, Bayern
  augsburg, Augsburg
  bamberg, Bamberg
  fuerstenzell, Fürstenzell
  hof, Hof
  hohenpeissenberg, Hohenpeißenberg
  kempten, Kempten (Allgäu)
  muenchen, München (Flughafen)
  nuernberg, Nürnberg (Flughafen)
  oberstdorf, Oberstdorf (Allgäu)
  regensburg, Regensburg
  straubing, Straubing
  weiden, Weiden in der Oberpfalz
  wuerzburg, Würzburg
berlin_brandenburg, Berlin und Brandenburg
  angermuende, Angermünde
  berlin, Berlin-Tempelhof
  cottbus, Cottbus
  lindenberg, Lindenberg (Tauche)
  neuruppin, Neuruppin
  potsdam, Potsdam
hessen, Hessen
  frankfurt, Frankfurt/Main (Flughafen)
  fritzlar, Fritzlar
  offenbach, Offenbach/Main (Wetterpark)
  wasserkuppe, Wasserkuppe
  wettenberg, Wettenberg bei Gießen
mecklenburg-vorpommern, Mecklenburg-Vorpommern
  arkona, Arkona (Putgarten)
  greifswald, Greifswald
  marnitz, Marnitz
  rostock, Rostock-Warnemünde
  schwerin, Schwerin
niedersachsen_bremen, Niedersachsen und Bremen
  bremen, Bremen (Flughafen)
  cuxhaven, Cuxhaven
  emden, Emden
  hannover, Hannover (Flughafen)
  norderney, Norderney
nordrhein-westfalen, Nordrhein-Westfalen
  aachen, Aachen-Orsbach
  lippspringe, Bad Lippspringe
  duesseldorf, Düsseldorf (Flughafen)
  greven, Greven (Flughafen Münster/Osnabrück)
  kahler_asten, Kahler Asten (Winterberg)
  koeln, Köln (Flughafen Köln/Bonn)
rheinland-pfalz_saarland, Rheinland-Pfalz und Saarland
  hahn, Hahn (Flughafen)
  nuerburg, Nürburg-Barweiler
  saarbruecken, Saarbrücken (Flughafen)
  trier, Trier-Petrisberg
sachsen, Sachsen Sachsen-Anhalt
  dresden, Dresden (Flughafen)
  goerlitz, Görlitz
  leipzig, Leipzig (Flughafen Leipzig/Halle)
sachsen-anhalt, Sachsen-Anhalt
  magdeburg, Magdeburg
schleswig-holstein_hamburg, Schleswig-Holstein und Hamburg
  fehmarn, Fehmarn
  hamburg, Hamburg (Flughafen)
  helgoland, Helgoland
  kiel, Kiel-Holtenau
  list, List auf Sylt
  schleswig, Schleswig
thueringen, Thüringen
  erfurt, Erfurt (Flughafen Erfurt-Weimar)
  gera, Gera
  meiningen, Meiningen


