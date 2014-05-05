--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: test; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE test (
    test_id integer NOT NULL,
    value_string character varying(50),
    val_int integer,
    val_float double precision,
    val_timestamp timestamp without time zone
);


ALTER TABLE public.test OWNER TO postgres;

--
-- Name: test_child_1; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE test_child_1 (
    teste_child_1_id integer NOT NULL,
    key_parent integer NOT NULL,
    val_str character varying(100),
    val_str1 character varying(100)
);


ALTER TABLE public.test_child_1 OWNER TO postgres;

--
-- Name: test_child_1_teste_child_1_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE test_child_1_teste_child_1_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.test_child_1_teste_child_1_id_seq OWNER TO postgres;

--
-- Name: test_child_1_teste_child_1_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE test_child_1_teste_child_1_id_seq OWNED BY test_child_1.teste_child_1_id;


--
-- Name: test_child_1_teste_child_1_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('test_child_1_teste_child_1_id_seq', 1, false);


--
-- Name: test_child_blob; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE test_child_blob (
    teste_child_blob_id integer NOT NULL,
    key_parent integer NOT NULL,
    val_str character varying(100),
    val_blob bytea
);


ALTER TABLE public.test_child_blob OWNER TO postgres;

--
-- Name: test_child_blob_teste_child_blob_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE test_child_blob_teste_child_blob_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.test_child_blob_teste_child_blob_id_seq OWNER TO postgres;

--
-- Name: test_child_blob_teste_child_blob_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE test_child_blob_teste_child_blob_id_seq OWNED BY test_child_blob.teste_child_blob_id;


--
-- Name: test_child_blob_teste_child_blob_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('test_child_blob_teste_child_blob_id_seq', 1, false);


--
-- Name: test_child_multi_col; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE test_child_multi_col (
    teste_child_multi_col_id integer NOT NULL,
    key_parent integer NOT NULL,
    val_str character varying(100),
    val_str1 character varying(100),
    val_str2 character varying(20),
    val_str3 character varying(50),
    val_str4 character varying(20),
    val_str5 character varying(20),
    val_str6 character varying(50),
    val_int integer,
    val_int1 integer,
    val_int2 integer,
    val_int3 integer,
    val_int4 integer,
    val_float double precision,
    val_float1 double precision,
    val_float2 double precision
);


ALTER TABLE public.test_child_multi_col OWNER TO postgres;

--
-- Name: test_child_multi_col_teste_child_multi_col_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE test_child_multi_col_teste_child_multi_col_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.test_child_multi_col_teste_child_multi_col_id_seq OWNER TO postgres;

--
-- Name: test_child_multi_col_teste_child_multi_col_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE test_child_multi_col_teste_child_multi_col_id_seq OWNED BY test_child_multi_col.teste_child_multi_col_id;


--
-- Name: test_child_multi_col_teste_child_multi_col_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('test_child_multi_col_teste_child_multi_col_id_seq', 1, false);


--
-- Name: test_test_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE test_test_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.test_test_id_seq OWNER TO postgres;

--
-- Name: test_test_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE test_test_id_seq OWNED BY test.test_id;


--
-- Name: test_test_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('test_test_id_seq', 1, false);


--
-- Name: test_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY test ALTER COLUMN test_id SET DEFAULT nextval('test_test_id_seq'::regclass);


--
-- Name: teste_child_1_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY test_child_1 ALTER COLUMN teste_child_1_id SET DEFAULT nextval('test_child_1_teste_child_1_id_seq'::regclass);


--
-- Name: teste_child_blob_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY test_child_blob ALTER COLUMN teste_child_blob_id SET DEFAULT nextval('test_child_blob_teste_child_blob_id_seq'::regclass);


--
-- Name: teste_child_multi_col_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY test_child_multi_col ALTER COLUMN teste_child_multi_col_id SET DEFAULT nextval('test_child_multi_col_teste_child_multi_col_id_seq'::regclass);


--
-- Data for Name: test; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY test (test_id, value_string, val_int, val_float, val_timestamp) FROM stdin;
\.


--
-- Data for Name: test_child_1; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY test_child_1 (teste_child_1_id, key_parent, val_str, val_str1) FROM stdin;
\.


--
-- Data for Name: test_child_blob; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY test_child_blob (teste_child_blob_id, key_parent, val_str, val_blob) FROM stdin;
\.


--
-- Data for Name: test_child_multi_col; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY test_child_multi_col (teste_child_multi_col_id, key_parent, val_str, val_str1, val_str2, val_str3, val_str4, val_str5, val_str6, val_int, val_int1, val_int2, val_int3, val_int4, val_float, val_float1, val_float2) FROM stdin;
\.


--
-- Name: test_child_1_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY test_child_1
    ADD CONSTRAINT test_child_1_pkey PRIMARY KEY (teste_child_1_id);


--
-- Name: test_child_blob_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY test_child_blob
    ADD CONSTRAINT test_child_blob_pkey PRIMARY KEY (teste_child_blob_id);


--
-- Name: test_child_multi_col_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY test_child_multi_col
    ADD CONSTRAINT test_child_multi_col_pkey PRIMARY KEY (teste_child_multi_col_id);


--
-- Name: test_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY test
    ADD CONSTRAINT test_pkey PRIMARY KEY (test_id);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

