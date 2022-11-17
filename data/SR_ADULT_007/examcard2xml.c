#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

size_t b64_decoded_size(const char *in) {
  size_t len;
  size_t ret;
  size_t i;

  if (in == NULL)
    return 0;

  len = strlen(in);
  ret = len / 4 * 3;

  for (i = len; i-- > 0;) {
    if (in[i] == '=') {
      ret--;
    } else {
      break;
    }
  }

  return ret;
}

int b64invs[] = {62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                 61, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,
                 6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27,
                 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};

int b64_isvalidchar(char c) {
  if (c >= '0' && c <= '9')
    return 1;
  if (c >= 'A' && c <= 'Z')
    return 1;
  if (c >= 'a' && c <= 'z')
    return 1;
  if (c == '+' || c == '/' || c == '=')
    return 1;
  return 0;
}

int b64_decode(const char *in, unsigned char *out, size_t outlen) {
  size_t len;
  size_t i;
  size_t j;
  int v;

  if (in == NULL || out == NULL)
    return 0;

  len = strlen(in);
  if (outlen < b64_decoded_size(in) || len % 4 != 0)
    return 0;

  for (i = 0; i < len; i++) {
    if (!b64_isvalidchar(in[i])) {
      return 0;
    }
  }

  for (i = 0, j = 0; i < len; i += 4, j += 3) {
    v = b64invs[in[i] - 43];
    v = (v << 6) | b64invs[in[i + 1] - 43];
    v = in[i + 2] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 2] - 43];
    v = in[i + 3] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 3] - 43];

    out[j] = (v >> 16) & 0xFF;
    if (in[i + 2] != '=')
      out[j + 1] = (v >> 8) & 0xFF;
    if (in[i + 3] != '=')
      out[j + 2] = v & 0xFF;
  }

  return 1;
}

// Need LIBXML_XPATH_ENABLED & LIBXML_SAX1_ENABLED

static void usage(const char *name);
int execute_xpath_expression(const char *filename, const xmlChar *xpathExpr0,
                             const xmlChar *xpathExpr, const xmlChar *nsList);
int register_namespaces(xmlXPathContextPtr xpathCtx, const xmlChar *nsList);
void print_xpath_nodes(xmlNodeSetPtr nodes0, xmlNodeSetPtr nodes, FILE *output);

static const char old_style[] =
    "http://schemas.microsoft.com/clr/nsassem/Philips.PmsMR.Acquisition.AcqGlo/"
    "philips.pmsmr.acquisition.acqglo_cs";
static const char new_style[] =
    "http://schemas.microsoft.com/clr/nsassem/Philips.PmsMR.ExamCards.Model/"
    "philips.pmsmr.examcards.model.scanproperties_cs";

int find_type(const char *filename) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(filename, "r");
  if (fp == NULL)
    return -1;

  int type = -1;
  const char needle[] = ":ScanProcedure id=";
  // TODO: redo with fgets
  while ((read = getline(&line, &len, fp)) != -1) {
    //	  assert( read < 512*8 ); // FIXME: <SOAP-ENC:Array can be very very
    //long
    char *ptr = strstr(line, needle);
    if (ptr) {
      if (strstr(line, old_style))
        type = 0;
      else if (strstr(line, new_style))
        type = 1;
      break;
    }
  }

  fclose(fp);
  if (line)
    free(line);
  return type;
}

int main(int argc, char **argv) {
  const char *filename = argv[1];
  const xmlChar *xpathExpr0 = "//SOAP-ENC:Array";
  const xmlChar *xpathExpr = "//foo:ScanProcedure";
  const xmlChar *nsList = NULL;
  // old style:
  const xmlChar *nsListOld =
      "foo=http://schemas.microsoft.com/clr/nsassem/"
      "Philips.PmsMR.Acquisition.AcqGlo/philips.pmsmr.acquisition.acqglo_cs "
      "SOAP-ENC=http://schemas.xmlsoap.org/soap/encoding/";
  // new style
  const xmlChar *nsListNew =
      "foo=http://schemas.microsoft.com/clr/nsassem/"
      "Philips.PmsMR.ExamCards.Model/"
      "philips.pmsmr.examcards.model.scanproperties_cs "
      "SOAP-ENC=http://schemas.xmlsoap.org/soap/encoding/";

  int type = find_type(filename);
  if (type < 0)
    return -1;

  if (type == 0) {
    nsList = nsListOld;
  } else if (type == 1) {
    nsList = nsListNew;
  } else
    return -1;

  /* Init libxml */
  xmlInitParser();
  LIBXML_TEST_VERSION

  /* Do the main job */
  if (execute_xpath_expression(filename, xpathExpr0, xpathExpr, nsList) < 0) {
    usage(argv[0]);
    return (-1);
  }

  /* Shutdown libxml */
  xmlCleanupParser();

  return 0;
}

/**
 * usage:
 * @name:		the program name.
 *
 * Prints usage information.
 */
static void usage(const char *name) {
  assert(name);

  fprintf(stderr, "Usage: %s <xml-file> <xpath-expr> [<known-ns-list>]\n",
          name);
  fprintf(stderr, "where <known-ns-list> is a list of known namespaces\n");
  fprintf(stderr, "in \"<prefix1>=<href1> <prefix2>=href2> ...\" format\n");
}

/**
 * execute_xpath_expression:
 * @filename:		the input XML filename.
 * @xpathExpr:		the xpath expression for evaluation.
 * @nsList:		the optional list of known namespaces in
 *			"<prefix1>=<href1> <prefix2>=href2> ..." format.
 *
 * Parses input XML file, evaluates XPath expression and prints results.
 *
 * Returns 0 on success and a negative value otherwise.
 */
int execute_xpath_expression(const char *filename, const xmlChar *xpathExpr0,
                             const xmlChar *xpathExpr, const xmlChar *nsList) {
  xmlDocPtr doc;
  xmlXPathContextPtr xpathCtx;
  xmlXPathObjectPtr xpathObj;
  xmlXPathObjectPtr xpathObj0;

  assert(filename);
  assert(xpathExpr);

  /* Load XML document */
  doc = xmlParseFile(filename);
  if (doc == NULL) {
    fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
    return (-1);
  }

  /* Create xpath evaluation context */
  xpathCtx = xmlXPathNewContext(doc);
  if (xpathCtx == NULL) {
    fprintf(stderr, "Error: unable to create new XPath context\n");
    xmlFreeDoc(doc);
    return (-1);
  }

  /* Register namespaces from list (if any) */
  if ((nsList != NULL) && (register_namespaces(xpathCtx, nsList) < 0)) {
    fprintf(stderr, "Error: failed to register namespaces list \"%s\"\n",
            nsList);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    return (-1);
  }

  /* Evaluate xpath expression */
  xpathObj0 = xmlXPathEvalExpression(xpathExpr0, xpathCtx);
  if (xpathObj0 == NULL) {
    fprintf(stderr, "Error: unable to evaluate xpath expression \"%s\"\n",
            xpathExpr);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    return (-1);
  }

  /* Evaluate xpath expression */
  xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
  if (xpathObj == NULL) {
    fprintf(stderr, "Error: unable to evaluate xpath expression \"%s\"\n",
            xpathExpr);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
    return (-1);
  }

  /* Print results */
  print_xpath_nodes(xpathObj0->nodesetval, xpathObj->nodesetval, stdout);

  /* Cleanup */
  xmlXPathFreeObject(xpathObj0);
  xmlXPathFreeObject(xpathObj);
  xmlXPathFreeContext(xpathCtx);
  xmlFreeDoc(doc);

  return (0);
}

/**
 * register_namespaces:
 * @xpathCtx:		the pointer to an XPath context.
 * @nsList:		the list of known namespaces in
 *			"<prefix1>=<href1> <prefix2>=href2> ..." format.
 *
 * Registers namespaces from @nsList in @xpathCtx.
 *
 * Returns 0 on success and a negative value otherwise.
 */
int register_namespaces(xmlXPathContextPtr xpathCtx, const xmlChar *nsList) {
  xmlChar *nsListDup;
  xmlChar *prefix;
  xmlChar *href;
  xmlChar *next;

  assert(xpathCtx);
  assert(nsList);

  nsListDup = xmlStrdup(nsList);
  if (nsListDup == NULL) {
    fprintf(stderr, "Error: unable to strdup namespaces list\n");
    return (-1);
  }

  next = nsListDup;
  while (next != NULL) {
    /* skip spaces */
    while ((*next) == ' ')
      next++;
    if ((*next) == '\0')
      break;

    /* find prefix */
    prefix = next;
    next = (xmlChar *)xmlStrchr(next, '=');
    if (next == NULL) {
      fprintf(stderr, "Error: invalid namespaces list format\n");
      xmlFree(nsListDup);
      return (-1);
    }
    *(next++) = '\0';

    /* find href */
    href = next;
    next = (xmlChar *)xmlStrchr(next, ' ');
    if (next != NULL) {
      *(next++) = '\0';
    }

    /* do register namespace */
    if (xmlXPathRegisterNs(xpathCtx, prefix, href) != 0) {
      fprintf(
          stderr,
          "Error: unable to register NS with prefix=\"%s\" and href=\"%s\"\n",
          prefix, href);
      xmlFree(nsListDup);
      return (-1);
    }
  }

  xmlFree(nsListDup);
  return (0);
}

void process_examcard_data(const void *data, size_t len);

void get_xpath_nodes(xmlChar *name, xmlNodeSetPtr nodes, const char *href_id) {
  xmlNodePtr cur;
  int size;
  int i;

  size = (nodes) ? nodes->nodeNr : 0;
  FILE *output = stdout;
  char buffer[512];
  assert(strlen(name) < 512);
  sprintf(buffer, "%s.raw", name);
  for (int i = 0; i < strlen(name); ++i) {
    if (buffer[i] == '/')
      buffer[i] = '|';
  }
  FILE *stream = fopen(buffer, "wb");
  assert(stream);

  fprintf(output, "Result (%d nodes):\n", size);
  for (i = 0; i < size; ++i) {
    assert(nodes->nodeTab[i]);
    assert(nodes->nodeTab[i]->type == XML_ELEMENT_NODE);
    cur = nodes->nodeTab[i];
    assert(cur->ns);
    if (cur->ns) {
      xmlChar *prop = xmlGetProp(cur, "id");
      if (strcmp(prop, href_id + 1) == 0) {
        xmlChar *data = xmlNodeGetContent(cur);
        char *input = data;
        unsigned long inlen = strlen(data);
        size_t dlen = b64_decoded_size(
            input) /* gdcm::Base64::GetDecodeLength( input, inlen )*/;
        void *raw = malloc(dlen);
        //	  gdcm::Base64::Decode( raw, dlen, input, inlen );
        int r64 = b64_decode(input, raw, dlen);
        assert(r64 == 1);
        process_examcard_data(raw, dlen);
        assert(stream);
        fwrite(raw, 1, dlen, stream);
        free(raw);
        xmlFree(data);
      }
      xmlFree(prop);
    }
  }
  fclose(stream);
}

/**
 * print_element_names:
 * @a_node: the initial xml node to consider.
 *
 * Prints the names of the all the xml elements
 * that are siblings or children of a given xml node.
 */
static void print_element_names(xmlNodeSetPtr nodes0, xmlNode *a_node) {
  xmlNode *cur_node = NULL;

  // name
  xmlChar *name = NULL;
  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      if (strcmp(cur_node->name, "name") == 0) {
        assert(name == NULL);
        name = xmlNodeGetContent(cur_node);
      }
    }
  }
  // data
  xmlChar *prop = NULL;
  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      if (strcmp(cur_node->name, "parameterData") == 0) {
        assert(prop == NULL);
        prop = xmlGetProp(cur_node, "href");
      }
    }
  }
  // printf("node type: Element, name 2: %s %s\n", cur_node->name, prop);
  get_xpath_nodes(name, nodes0, prop);
  xmlFree(prop);
  xmlFree(name);
}

/**
 * print_xpath_nodes:
 * @nodes:		the nodes set.
 * @output:		the output file handle.
 *
 * Prints the @nodes content to @output.
 */
void print_xpath_nodes(xmlNodeSetPtr nodes0, xmlNodeSetPtr nodes,
                       FILE *output) {
  xmlNodePtr cur;
  int size;
  int i;

  assert(output);
  size = (nodes) ? nodes->nodeNr : 0;

  fprintf(output, "Result (%d nodes):\n", size);
  for (i = 0; i < size; ++i) {
    assert(nodes->nodeTab[i]);

    if (nodes->nodeTab[i]->type == XML_NAMESPACE_DECL) {
      assert(0);
      xmlNsPtr ns;

      ns = (xmlNsPtr)nodes->nodeTab[i];
      cur = (xmlNodePtr)ns->next;
      if (cur->ns) {
        fprintf(output, "= namespace \"%s\"=\"%s\" for node %s:%s\n",
                ns->prefix, ns->href, cur->ns->href, cur->name);
      } else {
        fprintf(output, "= namespace \"%s\"=\"%s\" for node %s\n", ns->prefix,
                ns->href, cur->name);
      }
    } else if (nodes->nodeTab[i]->type == XML_ELEMENT_NODE) {
      cur = nodes->nodeTab[i];
      if (cur->ns) {
        print_element_names(nodes0, cur->children);
      } else {
        fprintf(output, "= element node \"%s\"\n", cur->name);
      }
    } else {
      assert(0);
      cur = nodes->nodeTab[i];
      fprintf(output, "= node \"%s\": type %d\n", cur->name, cur->type);
    }
  }
}
