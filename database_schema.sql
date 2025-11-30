--------------------------------------------------------
-- SCRIPT SQL : Source_Projet2A - Schéma SYSTEM (MIS À JOUR)
-- Tables : vehicule, habitat, habitant, zone, poubelle, personnel, zone_verte, 
--          utilisation_vehicule, MAINTENANCE_HISTORY, CONSOMMATION
-- Liaisons : habitant ↔ habitat ↔ zone ; habitant ↔ vehicule
-- Réexécutable – Date : 08 novembre 2025
--------------------------------------------------------

--------------------------------------------------------
-- 1. SUPPRESSION DES OBJETS EXISTANTS
--------------------------------------------------------

BEGIN EXECUTE IMMEDIATE 'DROP TABLE SYSTEM.utilisation_vehicule CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE SYSTEM.MAINTENANCE_HISTORY CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE SYSTEM.CONSOMMATION CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE SYSTEM.vehicule CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE SYSTEM.habitat CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE SYSTEM.habitant CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE SYSTEM.zone CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE SYSTEM.poubelle CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE SYSTEM.personnel CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE SYSTEM.zone_verte CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/

-- Séquences
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SYSTEM.seq_vehicule'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SYSTEM.seq_habitat'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SYSTEM.seq_habitant'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SYSTEM.seq_zone'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SYSTEM.seq_poubelle'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SYSTEM.seq_zone_verte'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SYSTEM.seq_utilisation'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SYSTEM.seq_maintenance_history'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SYSTEM.seq_consommation'; EXCEPTION WHEN OTHERS THEN NULL; END;
/

-- Triggers
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER SYSTEM.trg_vehicule_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER SYSTEM.trg_habitat_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER SYSTEM.trg_habitant_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER SYSTEM.trg_zone_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER SYSTEM.trg_poubelle_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER SYSTEM.trg_zone_verte_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER SYSTEM.trg_utilisation_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER SYSTEM.trg_maintenance_history_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TRIGGER SYSTEM.trg_consommation_id'; EXCEPTION WHEN OTHERS THEN NULL; END;
/

--------------------------------------------------------
-- 2. CREATION DES TABLES
--------------------------------------------------------

CREATE TABLE SYSTEM.poubelle (
    id_poubelle NUMBER,
    capacite VARCHAR2(100),
    status VARCHAR2(50)
);

CREATE TABLE SYSTEM.zone (
    id_zone NUMBER,
    population NUMBER,
    x NUMBER,
    y NUMBER,
    l NUMBER,
    h NUMBER,
    surface NUMBER,
    densite NUMBER,
    id_poubelle NUMBER
);

CREATE TABLE SYSTEM.zone_verte (
    id_zone_verte NUMBER,
    nom VARCHAR2(100),
    superficie NUMBER,
    id_zone NUMBER
);

CREATE TABLE SYSTEM.personnel (
    cin NUMBER,
    nom VARCHAR2(50) NOT NULL,
    prenom VARCHAR2(50) NOT NULL,
    zone_affectation VARCHAR2(50),
    disponibilite VARCHAR2(30),
    competences VARCHAR2(100)
);

CREATE TABLE SYSTEM.habitant (
    id_habitant NUMBER,
    nom VARCHAR2(50) NOT NULL,
    prenom VARCHAR2(50) NOT NULL,
    adresse VARCHAR2(100),
    zone_residence VARCHAR2(50),
    situation_familiale VARCHAR2(50),
    contact VARCHAR2(30),
    statut VARCHAR2(20),
    besoins_specifiques VARCHAR2(200),
    niveau_satisfaction VARCHAR2(20)
);

CREATE TABLE SYSTEM.habitat (
    id_habitat NUMBER,
    type_logement VARCHAR2(50),
    surface NUMBER,
    etat VARCHAR2(30),
    id_habitant NUMBER,
    id_zone NUMBER
);

CREATE TABLE SYSTEM.vehicule (
    id_vehicule NUMBER,
    type_vehicule VARCHAR2(50),
    matricule VARCHAR2(20),
    capacite VARCHAR2(20),
    itineraire VARCHAR2(150),
    heure_depart VARCHAR2(20),
    heure_arrivee VARCHAR2(20),
    statut VARCHAR2(30),
    zone VARCHAR2(50),
    id_habitant NUMBER -- ↔ clé étrangère directe vers HABITANT
);

CREATE TABLE SYSTEM.utilisation_vehicule (
    id_utilisation NUMBER,
    id_habitant NUMBER NOT NULL,
    id_vehicule NUMBER NOT NULL,
    date_utilisation DATE DEFAULT SYSDATE,
    heure_debut VARCHAR2(20),
    heure_fin VARCHAR2(20)
);

-- Table MAINTENANCE_HISTORY (exact match with maintenance.cpp)
CREATE TABLE SYSTEM.MAINTENANCE_HISTORY (
    ID_MAINTENANCE NUMBER,
    ID_VEHICULE NUMBER NOT NULL,
    MATRICULE VARCHAR2(20),
    PROBLEME VARCHAR2(200),
    ALERTE VARCHAR2(500),
    DIAGNOSTIC VARCHAR2(2000),
    DATE_SAISIE DATE DEFAULT SYSDATE
);

-- Table CONSOMMATION (exact match with consomation.cpp)
CREATE TABLE SYSTEM.CONSOMMATION (
    ID_CONSOMMATION NUMBER,
    ID_VEHICULE NUMBER NOT NULL,
    MATRICULE VARCHAR2(20),
    DISTANCE_KM NUMBER,
    CONSO_ENTREE NUMBER,
    RESULTAT_CALCULE NUMBER,
    DATE_SAISIE DATE DEFAULT SYSDATE
);

--------------------------------------------------------
-- 3. CREATION DES SEQUENCES
--------------------------------------------------------

CREATE SEQUENCE SYSTEM.seq_poubelle START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SYSTEM.seq_zone START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SYSTEM.seq_zone_verte START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SYSTEM.seq_habitant START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SYSTEM.seq_habitat START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SYSTEM.seq_vehicule START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SYSTEM.seq_utilisation START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SYSTEM.seq_maintenance_history START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SYSTEM.seq_consommation START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;

--------------------------------------------------------
-- 4. CREATION DES TRIGGERS
--------------------------------------------------------

CREATE OR REPLACE TRIGGER SYSTEM.trg_poubelle_id
BEFORE INSERT ON SYSTEM.poubelle
FOR EACH ROW
WHEN (NEW.id_poubelle IS NULL)
BEGIN
  SELECT SYSTEM.seq_poubelle.NEXTVAL INTO :NEW.id_poubelle FROM DUAL;
END;
/

CREATE OR REPLACE TRIGGER SYSTEM.trg_zone_id
BEFORE INSERT ON SYSTEM.zone
FOR EACH ROW
WHEN (NEW.id_zone IS NULL)
BEGIN
  SELECT SYSTEM.seq_zone.NEXTVAL INTO :NEW.id_zone FROM DUAL;
END;
/

CREATE OR REPLACE TRIGGER SYSTEM.trg_zone_verte_id
BEFORE INSERT ON SYSTEM.zone_verte
FOR EACH ROW
WHEN (NEW.id_zone_verte IS NULL)
BEGIN
  SELECT SYSTEM.seq_zone_verte.NEXTVAL INTO :NEW.id_zone_verte FROM DUAL;
END;
/

CREATE OR REPLACE TRIGGER SYSTEM.trg_habitant_id
BEFORE INSERT ON SYSTEM.habitant
FOR EACH ROW
WHEN (NEW.id_habitant IS NULL)
BEGIN
  SELECT SYSTEM.seq_habitant.NEXTVAL INTO :NEW.id_habitant FROM DUAL;
END;
/

CREATE OR REPLACE TRIGGER SYSTEM.trg_habitat_id
BEFORE INSERT ON SYSTEM.habitat
FOR EACH ROW
WHEN (NEW.id_habitat IS NULL)
BEGIN
  SELECT SYSTEM.seq_habitat.NEXTVAL INTO :NEW.id_habitat FROM DUAL;
END;
/

CREATE OR REPLACE TRIGGER SYSTEM.trg_vehicule_id
BEFORE INSERT ON SYSTEM.vehicule
FOR EACH ROW
WHEN (NEW.id_vehicule IS NULL)
BEGIN
  SELECT SYSTEM.seq_vehicule.NEXTVAL INTO :NEW.id_vehicule FROM DUAL;
END;
/

CREATE OR REPLACE TRIGGER SYSTEM.trg_utilisation_id
BEFORE INSERT ON SYSTEM.utilisation_vehicule
FOR EACH ROW
WHEN (NEW.id_utilisation IS NULL)
BEGIN
  SELECT SYSTEM.seq_utilisation.NEXTVAL INTO :NEW.id_utilisation FROM DUAL;
END;
/

CREATE OR REPLACE TRIGGER SYSTEM.trg_maintenance_history_id
BEFORE INSERT ON SYSTEM.MAINTENANCE_HISTORY
FOR EACH ROW
WHEN (NEW.ID_MAINTENANCE IS NULL)
BEGIN
  SELECT SYSTEM.seq_maintenance_history.NEXTVAL INTO :NEW.ID_MAINTENANCE FROM DUAL;
END;
/

CREATE OR REPLACE TRIGGER SYSTEM.trg_consommation_id
BEFORE INSERT ON SYSTEM.CONSOMMATION
FOR EACH ROW
WHEN (NEW.ID_CONSOMMATION IS NULL)
BEGIN
  SELECT SYSTEM.seq_consommation.NEXTVAL INTO :NEW.ID_CONSOMMATION FROM DUAL;
END;
/

--------------------------------------------------------
-- 5. CONTRAINTES
--------------------------------------------------------

-- Primary Keys
ALTER TABLE SYSTEM.poubelle ADD CONSTRAINT pk_poubelle PRIMARY KEY (id_poubelle);
ALTER TABLE SYSTEM.zone ADD CONSTRAINT pk_zone PRIMARY KEY (id_zone);
ALTER TABLE SYSTEM.zone_verte ADD CONSTRAINT pk_zone_verte PRIMARY KEY (id_zone_verte);
ALTER TABLE SYSTEM.personnel ADD CONSTRAINT pk_personnel PRIMARY KEY (cin);
ALTER TABLE SYSTEM.habitant ADD CONSTRAINT pk_habitant PRIMARY KEY (id_habitant);
ALTER TABLE SYSTEM.habitat ADD CONSTRAINT pk_habitat PRIMARY KEY (id_habitat);
ALTER TABLE SYSTEM.vehicule ADD CONSTRAINT pk_vehicule PRIMARY KEY (id_vehicule);
ALTER TABLE SYSTEM.utilisation_vehicule ADD CONSTRAINT pk_utilisation PRIMARY KEY (id_utilisation);
ALTER TABLE SYSTEM.MAINTENANCE_HISTORY ADD CONSTRAINT pk_maintenance_history PRIMARY KEY (ID_MAINTENANCE);
ALTER TABLE SYSTEM.CONSOMMATION ADD CONSTRAINT pk_consommation PRIMARY KEY (ID_CONSOMMATION);

-- Foreign Keys
ALTER TABLE SYSTEM.zone ADD CONSTRAINT fk_zone_poubelle
    FOREIGN KEY (id_poubelle) REFERENCES SYSTEM.poubelle(id_poubelle);

ALTER TABLE SYSTEM.zone_verte ADD CONSTRAINT fk_zone_verte_zone
    FOREIGN KEY (id_zone) REFERENCES SYSTEM.zone(id_zone);

ALTER TABLE SYSTEM.habitat ADD CONSTRAINT fk_habitat_habitant
    FOREIGN KEY (id_habitant) REFERENCES SYSTEM.habitant(id_habitant);

ALTER TABLE SYSTEM.habitat ADD CONSTRAINT fk_habitat_zone
    FOREIGN KEY (id_zone) REFERENCES SYSTEM.zone(id_zone);

-- Direct relation VEHICULE ↔ HABITANT
ALTER TABLE SYSTEM.vehicule ADD CONSTRAINT fk_vehicule_habitant
    FOREIGN KEY (id_habitant) REFERENCES SYSTEM.habitant(id_habitant);

-- Liaison utilisation
ALTER TABLE SYSTEM.utilisation_vehicule ADD CONSTRAINT fk_util_habitant
    FOREIGN KEY (id_habitant) REFERENCES SYSTEM.habitant(id_habitant);

ALTER TABLE SYSTEM.utilisation_vehicule ADD CONSTRAINT fk_util_vehicule
    FOREIGN KEY (id_vehicule) REFERENCES SYSTEM.vehicule(id_vehicule);

-- Foreign Keys for MAINTENANCE_HISTORY and CONSOMMATION
ALTER TABLE SYSTEM.MAINTENANCE_HISTORY ADD CONSTRAINT fk_maintenance_history_vehicule
    FOREIGN KEY (ID_VEHICULE) REFERENCES SYSTEM.vehicule(id_vehicule);

ALTER TABLE SYSTEM.CONSOMMATION ADD CONSTRAINT fk_consommation_vehicule
    FOREIGN KEY (ID_VEHICULE) REFERENCES SYSTEM.vehicule(id_vehicule);

-- CHECK Constraints
ALTER TABLE SYSTEM.habitant ADD CONSTRAINT ck_statut_habitant
    CHECK (statut IN ('Actif', 'Inactif'));

ALTER TABLE SYSTEM.vehicule ADD CONSTRAINT ck_type_vehicule
    CHECK (type_vehicule IN ('Bus', 'Tram', 'Électrique', 'Voiture'));

ALTER TABLE SYSTEM.vehicule ADD CONSTRAINT ck_statut_vehicule
    CHECK (statut IN ('En service', 'En recharge', 'En panne', 'Disponible'));

ALTER TABLE SYSTEM.personnel ADD CONSTRAINT ck_dispo_personnel
    CHECK (disponibilite IN ('Disponible', 'Indisponible'));


--------------------------------------------------------
-- 6. INDEX
--------------------------------------------------------

CREATE INDEX idx_habitant_zone ON SYSTEM.habitant(zone_residence);
CREATE INDEX idx_personnel_zone ON SYSTEM.personnel(zone_affectation);
CREATE INDEX idx_vehicule_zone ON SYSTEM.vehicule(zone);
CREATE INDEX idx_vehicule_habitant ON SYSTEM.vehicule(id_habitant);
CREATE INDEX idx_utilisation_habitant ON SYSTEM.utilisation_vehicule(id_habitant);
CREATE INDEX idx_utilisation_vehicule ON SYSTEM.utilisation_vehicule(id_vehicule);
CREATE INDEX idx_maintenance_history_vehicule ON SYSTEM.MAINTENANCE_HISTORY(ID_VEHICULE);
CREATE INDEX idx_maintenance_history_date ON SYSTEM.MAINTENANCE_HISTORY(DATE_SAISIE);
CREATE INDEX idx_consommation_vehicule ON SYSTEM.CONSOMMATION(ID_VEHICULE);
CREATE INDEX idx_consommation_date ON SYSTEM.CONSOMMATION(DATE_SAISIE);

--------------------------------------------------------
-- FIN DU SCRIPT
--------------------------------------------------------

