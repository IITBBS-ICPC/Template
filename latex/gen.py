import os
import argparse

def get_head_tail(args):
  head, tail = None, None
  with open(args.head) as f: head = f.read()
  with open(args.tail) as f: tail = f.read()
  return head, tail

def latex_string(s):
  res = ""
  for c in s:
    if c == "_":
      res+="\\_"
    else:
      res+=c
  return res

def main(args):
  tex = []
  head, tail = get_head_tail(args)
  tex.append(head)
  sections = os.listdir(args.code)
  for section in sections:
    tex.append("\\section{%s}" % latex_string(section))
    section_dir = os.path.join(args.code, section)
    sub_sections = os.listdir(section_dir)
    for sub_section in sub_sections:
      sub_section_name = ".".join(sub_section.split(".")[:-1])
      tex.append("\\subsection{%s}" % latex_string(sub_section_name))
      tex.append("\\lstinputlisting{%s}" % os.path.join(
        section_dir, sub_section))
  if args.theory:
    f = open(args.theory)
    tex.append(f.read())
  tex.append(tail)
  with open("temp.tex", "w") as f:
    f.write("\n".join(tex))
  os.system("pdflatex temp.tex")
  os.system("pdflatex temp.tex")

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--code", default="../codes")
  parser.add_argument("--head", default="head.tex.part")
  parser.add_argument("--theory", default="theory.tex.part")
  parser.add_argument("--tail", default="tail.tex.part")
  args = parser.parse_args()
  main(args)