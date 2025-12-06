------------------------------------------------------
-- PARTIE 0 (FACULTATIVE) : Créer HABITANT si absent
-- Exécuter cette partie seulement si la table HABITANT n'existe pas.
------------------------------------------------------
BEGIN EXECUTE IMMEDIATE 'DROP TABLE TOUHA.habitant CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE TOUHA.seq_habitant'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER TOUHA.trg_habitant_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/

CREATE TABLE TOUHA.habitant (
    id_habitant          NUMBER,
    nom                  VARCHAR2(50)   NOT NULL,
    prenom               VARCHAR2(50)   NOT NULL,
    adresse              VARCHAR2(100),
    zone_residence       VARCHAR2(50),
    situation_familiale  VARCHAR2(50),
    contact              VARCHAR2(30),
    statut               VARCHAR2(20)   DEFAULT 'Actif',
    besoins_specifiques  VARCHAR2(200),
    niveau_satisfaction  VARCHAR2(20)
);

CREATE SEQUENCE TOUHA.seq_habitant START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;

CREATE OR REPLACE TRIGGER TOUHA.trg_habitant_id
BEFORE INSERT ON TOUHA.habitant
FOR EACH ROW
WHEN (NEW.id_habitant IS NULL)
BEGIN
  SELECT TOUHA.seq_habitant.NEXTVAL INTO :NEW.id_habitant FROM DUAL;
END;
/

-- Aligner avec votre UI: accepte aussi 'En attente'
ALTER TABLE TOUHA.habitant ADD CONSTRAINT pk_habitant PRIMARY KEY (id_habitant);
ALTER TABLE TOUHA.habitant ADD CONSTRAINT ck_statut_habitant
  CHECK (statut IN ('Actif','Inactif','En attente'));
/

------------------------------------------------------
-- PARTIE 1 : DEMANDE (Table + Séquence + Triggers + Contraintes + Index + Vue)
------------------------------------------------------
BEGIN EXECUTE IMMEDIATE 'DROP VIEW TOUHA.vw_demande_detail'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP VIEW TOUHA.agg_demande_mensuelle'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE TOUHA.demande CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE TOUHA.seq_demande'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER TOUHA.trg_demande_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER TOUHA.trg_demande_bu'; EXCEPTION WHEN OTHERS THEN NULL; END;
/

-- Table DEMANDE
CREATE TABLE TOUHA.demande (
    id_demande     NUMBER,
    id_habitant    NUMBER NOT NULL,
    type_probleme  VARCHAR2(100) NOT NULL,
    description    CLOB NOT NULL,
    statut         VARCHAR2(20) DEFAULT 'Nouveau' NOT NULL,
    date_creation  DATE DEFAULT SYSDATE NOT NULL,
    date_maj       DATE
);

-- Séquence
CREATE SEQUENCE TOUHA.seq_demande START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;

-- Triggers auto ID et date_maj
CREATE OR REPLACE TRIGGER TOUHA.trg_demande_id
BEFORE INSERT ON TOUHA.demande
FOR EACH ROW
WHEN (NEW.id_demande IS NULL)
BEGIN
  SELECT TOUHA.seq_demande.NEXTVAL INTO :NEW.id_demande FROM DUAL;
  :NEW.date_creation := NVL(:NEW.date_creation, SYSDATE);
  :NEW.date_maj := SYSDATE;
END;
/

CREATE OR REPLACE TRIGGER TOUHA.trg_demande_bu
BEFORE UPDATE ON TOUHA.demande
FOR EACH ROW
BEGIN
  :NEW.date_maj := SYSDATE;
END;
/

-- Contraintes
ALTER TABLE TOUHA.demande ADD CONSTRAINT pk_demande PRIMARY KEY (id_demande);
ALTER TABLE TOUHA.demande ADD CONSTRAINT fk_demande_habitant
  FOREIGN KEY (id_habitant) REFERENCES TOUHA.habitant(id_habitant);
ALTER TABLE TOUHA.demande ADD CONSTRAINT ck_statut_demande
  CHECK (statut IN ('Nouveau','En_cours','Cloture'));

-- Index utiles
CREATE INDEX idx_demande_habitant ON TOUHA.demande(id_habitant);
CREATE INDEX idx_demande_statut   ON TOUHA.demande(statut);
CREATE INDEX idx_demande_date     ON TOUHA.demande(date_creation);

-- Vue “tableau demande” enrichie (avec nom/prénom habitant)
CREATE OR REPLACE VIEW TOUHA.vw_demande_detail AS
SELECT d.id_demande,
       d.id_habitant,
       h.nom,
       h.prenom,
       d.type_probleme,
       d.description,
       d.statut,
       d.date_creation,
       d.date_maj
FROM TOUHA.demande d
JOIN TOUHA.habitant h ON h.id_habitant = d.id_habitant;
/

-- Vue d'agrégat mensuel (analytique)
CREATE OR REPLACE VIEW TOUHA.agg_demande_mensuelle AS
SELECT TRUNC(date_creation,'MM') AS mois,
       COUNT(*) AS nb
FROM TOUHA.demande
GROUP BY TRUNC(date_creation,'MM');
/

------------------------------------------------------
-- PARTIE 2 : SONDAGE (Satisfaction des habitants)
-- Table + Séquence + Trigger + Index + Vues (CSAT, NPS, CES)
------------------------------------------------------
BEGIN EXECUTE IMMEDIATE 'DROP VIEW TOUHA.vw_sondage_recent'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP VIEW TOUHA.vw_ces'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP VIEW TOUHA.vw_nps'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP VIEW TOUHA.vw_csat'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE TOUHA.sondage CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE TOUHA.seq_sondage'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER TOUHA.trg_sondage_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/

CREATE TABLE TOUHA.sondage (
  id_sondage     NUMBER,
  date_rep       DATE DEFAULT SYSDATE NOT NULL,
  zone           VARCHAR2(50),
  service        VARCHAR2(50),
  csat           NUMBER,           -- 1..5
  ces            NUMBER,           -- 1..5 (effort)
  recommandation NUMBER,           -- 0..10 (NPS)
  commentaire    CLOB
);

CREATE SEQUENCE TOUHA.seq_sondage START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;

CREATE OR REPLACE TRIGGER TOUHA.trg_sondage_id
BEFORE INSERT ON TOUHA.sondage
FOR EACH ROW
WHEN (NEW.id_sondage IS NULL)
BEGIN
  SELECT TOUHA.seq_sondage.NEXTVAL INTO :NEW.id_sondage FROM DUAL;
END;
/

ALTER TABLE TOUHA.sondage ADD CONSTRAINT pk_sondage PRIMARY KEY (id_sondage);

CREATE INDEX TOUHA.idx_sondage_service ON TOUHA.sondage(service);
CREATE INDEX TOUHA.idx_sondage_zone    ON TOUHA.sondage(zone);
CREATE INDEX TOUHA.idx_sondage_date    ON TOUHA.sondage(date_rep);

-- CSAT: % de notes 4-5
CREATE OR REPLACE VIEW TOUHA.vw_csat AS
SELECT service,
       zone,
       TRUNC(date_rep,'MM') AS mois,
       ROUND(100*AVG(CASE WHEN csat>=4 THEN 1 ELSE 0 END),1) AS csat_pct
FROM TOUHA.sondage
GROUP BY service, zone, TRUNC(date_rep,'MM');
/

-- NPS: promoteurs(9-10) - detracteurs(0-6)
CREATE OR REPLACE VIEW TOUHA.vw_nps AS
SELECT service,
       zone,
       TRUNC(date_rep,'MM') AS mois,
       ROUND(100*AVG(CASE WHEN recommandation>=9 THEN 1 WHEN recommandation<=6 THEN -1 ELSE 0 END),1) AS nps_score
FROM TOUHA.sondage
GROUP BY service, zone, TRUNC(date_rep,'MM');
/

-- CES moyen (effort)
CREATE OR REPLACE VIEW TOUHA.vw_ces AS
SELECT service,
       zone,
       TRUNC(date_rep,'MM') AS mois,
       ROUND(AVG(ces),2) AS ces_moy
FROM TOUHA.sondage
GROUP BY service, zone, TRUNC(date_rep,'MM');
/

-- Détail des réponses récentes (3 derniers mois)
CREATE OR REPLACE VIEW TOUHA.vw_sondage_recent AS
SELECT id_sondage, date_rep, service, zone, csat, ces, recommandation, commentaire
FROM TOUHA.sondage
WHERE date_rep >= ADD_MONTHS(TRUNC(SYSDATE,'MM'), -3);
/

------------------------------------------------------
-- PARTIE 3 : Vue Satisfaction par Habitant + Demandes
-- Fournit pour chaque habitant:
--  - son niveau de satisfaction (HABITANT.NIVEAU_SATISFACTION)
--  - si au moins une demande est clôturée (demande_executée = 'Oui'/'Non')
--  - un type de demande clôturée (le plus récent si plusieurs), sinon 'Aucune demande'
------------------------------------------------------
BEGIN EXECUTE IMMEDIATE 'DROP VIEW TOUHA.vw_habitant_satisfaction'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
CREATE OR REPLACE VIEW TOUHA.vw_habitant_satisfaction AS
SELECT
  h.id_habitant,
  h.nom,
  h.besoins_specifiques AS besoins,
  h.niveau_satisfaction AS satisfaction,
  CASE WHEN SUM(CASE WHEN d.statut = 'Cloture' THEN 1 ELSE 0 END) > 0 THEN 'Oui' ELSE 'Non' END AS demande_executee,
  NVL(
    (SELECT d2.type_probleme
       FROM TOUHA.demande d2
      WHERE d2.id_habitant = h.id_habitant AND d2.statut = 'Cloture'
      ORDER BY d2.date_maj DESC NULLS LAST, d2.date_creation DESC NULLS LAST
      FETCH FIRST 1 ROWS ONLY),
    'Aucune demande'
  ) AS type_demande
FROM TOUHA.habitant h
LEFT JOIN TOUHA.demande d ON d.id_habitant = h.id_habitant
GROUP BY h.id_habitant, h.nom, h.besoins_specifiques, h.niveau_satisfaction;
/

------------------------------------------------------
-- PARTIE 4 : TABLE SATISFACTION (liée aux habitants)
------------------------------------------------------
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER TOUHA.trg_satisfaction_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE TOUHA.seq_satisfaction'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE TOUHA.satisfaction CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/

CREATE TABLE TOUHA.satisfaction (
  id_satisfaction NUMBER,
  id_habitant     NUMBER NOT NULL,
  id_demande      NUMBER,
  score           NUMBER(3) CHECK (score BETWEEN 0 AND 100),
  date_saisie     DATE DEFAULT SYSDATE NOT NULL,
  source          VARCHAR2(50),
  commentaire     CLOB
);

ALTER TABLE TOUHA.satisfaction ADD CONSTRAINT pk_satisfaction PRIMARY KEY (id_satisfaction);
ALTER TABLE TOUHA.satisfaction ADD CONSTRAINT fk_sat_habitant FOREIGN KEY (id_habitant) REFERENCES TOUHA.habitant(id_habitant);
ALTER TABLE TOUHA.satisfaction ADD CONSTRAINT fk_sat_demande  FOREIGN KEY (id_demande)  REFERENCES TOUHA.demande(id_demande);

CREATE INDEX TOUHA.idx_sat_habitant ON TOUHA.satisfaction(id_habitant);
CREATE INDEX TOUHA.idx_sat_demande  ON TOUHA.satisfaction(id_demande);
CREATE INDEX TOUHA.idx_sat_date     ON TOUHA.satisfaction(date_saisie);

CREATE SEQUENCE TOUHA.seq_satisfaction START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;

CREATE OR REPLACE TRIGGER TOUHA.trg_satisfaction_id
BEFORE INSERT ON TOUHA.satisfaction
FOR EACH ROW
WHEN (NEW.id_satisfaction IS NULL)
BEGIN
  SELECT TOUHA.seq_satisfaction.NEXTVAL INTO :NEW.id_satisfaction FROM DUAL;
END;
/