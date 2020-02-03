--
-- PostgreSQL database dump
--

-- Dumped from database version 10.10 (Ubuntu 10.10-0ubuntu0.18.04.1)
-- Dumped by pg_dump version 10.10 (Ubuntu 10.10-0ubuntu0.18.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: alternative; Type: TABLE; Schema: public; Owner: dirigente
--

CREATE TABLE public.alternative (
    idx integer NOT NULL,
    idx_general integer,
    value character varying(200)
);


ALTER TABLE public.alternative OWNER TO dirigente;

--
-- Name: alternative_idx_seq; Type: SEQUENCE; Schema: public; Owner: dirigente
--

CREATE SEQUENCE public.alternative_idx_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.alternative_idx_seq OWNER TO dirigente;

--
-- Name: alternative_idx_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: dirigente
--

ALTER SEQUENCE public.alternative_idx_seq OWNED BY public.alternative.idx;


--
-- Name: code; Type: TABLE; Schema: public; Owner: dirigente
--

CREATE TABLE public.code (
    idx integer NOT NULL,
    idx_general integer,
    code character varying(10)
);


ALTER TABLE public.code OWNER TO dirigente;

--
-- Name: code_idx_seq; Type: SEQUENCE; Schema: public; Owner: dirigente
--

CREATE SEQUENCE public.code_idx_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.code_idx_seq OWNER TO dirigente;

--
-- Name: code_idx_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: dirigente
--

ALTER SEQUENCE public.code_idx_seq OWNED BY public.code.idx;


--
-- Name: general; Type: TABLE; Schema: public; Owner: dirigente
--

CREATE TABLE public.general (
    idx integer NOT NULL,
    name character varying(100) NOT NULL,
    convocatory text,
    header text,
    type integer,
    active boolean
);


ALTER TABLE public.general OWNER TO dirigente;

--
-- Name: general_idx_seq; Type: SEQUENCE; Schema: public; Owner: dirigente
--

CREATE SEQUENCE public.general_idx_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.general_idx_seq OWNER TO dirigente;

--
-- Name: general_idx_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: dirigente
--

ALTER SEQUENCE public.general_idx_seq OWNED BY public.general.idx;


--
-- Name: minister; Type: TABLE; Schema: public; Owner: dirigente
--

CREATE TABLE public.minister (
    idx integer NOT NULL,
    idx_general integer,
    id integer,
    password character varying(10)
);


ALTER TABLE public.minister OWNER TO dirigente;

--
-- Name: minister_idx_seq; Type: SEQUENCE; Schema: public; Owner: dirigente
--

CREATE SEQUENCE public.minister_idx_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.minister_idx_seq OWNER TO dirigente;

--
-- Name: minister_idx_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: dirigente
--

ALTER SEQUENCE public.minister_idx_seq OWNED BY public.minister.idx;


--
-- Name: option; Type: TABLE; Schema: public; Owner: dirigente
--

CREATE TABLE public.option (
    idx integer NOT NULL,
    idx_general integer,
    quantity integer,
    allowed boolean
);


ALTER TABLE public.option OWNER TO dirigente;

--
-- Name: option_idx_seq; Type: SEQUENCE; Schema: public; Owner: dirigente
--

CREATE SEQUENCE public.option_idx_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.option_idx_seq OWNER TO dirigente;

--
-- Name: option_idx_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: dirigente
--

ALTER SEQUENCE public.option_idx_seq OWNED BY public.option.idx;


--
-- Name: people; Type: TABLE; Schema: public; Owner: dirigente
--

CREATE TABLE public.people (
    idx integer NOT NULL,
    idx_general integer,
    name character varying(200),
    email character varying(200),
    code character varying(10),
    used boolean
);


ALTER TABLE public.people OWNER TO dirigente;

--
-- Name: people_idx_seq; Type: SEQUENCE; Schema: public; Owner: dirigente
--

CREATE SEQUENCE public.people_idx_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.people_idx_seq OWNER TO dirigente;

--
-- Name: people_idx_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: dirigente
--

ALTER SEQUENCE public.people_idx_seq OWNED BY public.people.idx;


--
-- Name: state; Type: TABLE; Schema: public; Owner: dirigente
--

CREATE TABLE public.state (
    idx integer NOT NULL,
    idx_general integer,
    type integer,
    value integer
);


ALTER TABLE public.state OWNER TO dirigente;

--
-- Name: state_idx_seq; Type: SEQUENCE; Schema: public; Owner: dirigente
--

CREATE SEQUENCE public.state_idx_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.state_idx_seq OWNER TO dirigente;

--
-- Name: state_idx_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: dirigente
--

ALTER SEQUENCE public.state_idx_seq OWNED BY public.state.idx;


--
-- Name: vote; Type: TABLE; Schema: public; Owner: dirigente
--

CREATE TABLE public.vote (
    idx integer NOT NULL,
    idx_code integer,
    idx_alternative integer,
    value boolean
);


ALTER TABLE public.vote OWNER TO dirigente;

--
-- Name: vote_idx_seq; Type: SEQUENCE; Schema: public; Owner: dirigente
--

CREATE SEQUENCE public.vote_idx_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.vote_idx_seq OWNER TO dirigente;

--
-- Name: vote_idx_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: dirigente
--

ALTER SEQUENCE public.vote_idx_seq OWNED BY public.vote.idx;


--
-- Name: alternative idx; Type: DEFAULT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.alternative ALTER COLUMN idx SET DEFAULT nextval('public.alternative_idx_seq'::regclass);


--
-- Name: code idx; Type: DEFAULT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.code ALTER COLUMN idx SET DEFAULT nextval('public.code_idx_seq'::regclass);


--
-- Name: general idx; Type: DEFAULT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.general ALTER COLUMN idx SET DEFAULT nextval('public.general_idx_seq'::regclass);


--
-- Name: minister idx; Type: DEFAULT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.minister ALTER COLUMN idx SET DEFAULT nextval('public.minister_idx_seq'::regclass);


--
-- Name: option idx; Type: DEFAULT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.option ALTER COLUMN idx SET DEFAULT nextval('public.option_idx_seq'::regclass);


--
-- Name: people idx; Type: DEFAULT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.people ALTER COLUMN idx SET DEFAULT nextval('public.people_idx_seq'::regclass);


--
-- Name: state idx; Type: DEFAULT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.state ALTER COLUMN idx SET DEFAULT nextval('public.state_idx_seq'::regclass);


--
-- Name: vote idx; Type: DEFAULT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.vote ALTER COLUMN idx SET DEFAULT nextval('public.vote_idx_seq'::regclass);


--
-- Name: alternative alternative_pkey; Type: CONSTRAINT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.alternative
    ADD CONSTRAINT alternative_pkey PRIMARY KEY (idx);


--
-- Name: code code_pkey; Type: CONSTRAINT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.code
    ADD CONSTRAINT code_pkey PRIMARY KEY (idx);


--
-- Name: general general_pkey; Type: CONSTRAINT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.general
    ADD CONSTRAINT general_pkey PRIMARY KEY (idx);


--
-- Name: minister minister_pkey; Type: CONSTRAINT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.minister
    ADD CONSTRAINT minister_pkey PRIMARY KEY (idx);


--
-- Name: option option_pkey; Type: CONSTRAINT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.option
    ADD CONSTRAINT option_pkey PRIMARY KEY (idx);


--
-- Name: people people_pkey; Type: CONSTRAINT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.people
    ADD CONSTRAINT people_pkey PRIMARY KEY (idx);


--
-- Name: state state_pkey; Type: CONSTRAINT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.state
    ADD CONSTRAINT state_pkey PRIMARY KEY (idx);


--
-- Name: vote vote_pkey; Type: CONSTRAINT; Schema: public; Owner: dirigente
--

ALTER TABLE ONLY public.vote
    ADD CONSTRAINT vote_pkey PRIMARY KEY (idx);


--
-- PostgreSQL database dump complete
--

