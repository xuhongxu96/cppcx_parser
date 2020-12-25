using Antlr4.Runtime.Misc;
using Antlr4.Runtime.Tree;
using cppcx.Core.Models;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Analyzers
{
    public class GlobalVariableAnalyzer
    {
        public class GlobalVariable
        {
            public string Type { get; set; }
            public string Name { get; set; }
            public bool IsExtern { get; set; } = false;
            public CPPCXParser.InitializerContext InitContext { get; set; }
        }

        public IEnumerable<GlobalVariable> Visit([NotNull] CPPCXParser.SimpleDeclarationContext context)
        {
            var decls = context.declSpecifierSeq()?.declSpecifier();
            if (decls == null) yield break;

            var initDecls = context.initDeclaratorList()?.initDeclarator();

            if (initDecls == null)
            {
                Debug.Assert(decls.Length >= 2);

                var name = decls[^1].GetText();
                var type = decls[^2].GetText();
                var isExtern = false;

                if (decls.Length > 2)
                {
                    isExtern = decls[0].GetText() == "extern";
                }

                yield return new GlobalVariable
                {
                    Type = type,
                    Name = name,
                    IsExtern = isExtern,
                };
            }
            else
            {
                var type = decls[^1].GetText();
                var isExtern = false;

                if (decls.Length > 1)
                {
                    isExtern = decls[0].GetText() == "extern";
                }

                foreach (var initDecl in initDecls)
                {
                    yield return new GlobalVariable
                    {
                        Type = type,
                        Name = initDecl.declarator().GetText(),
                        IsExtern = isExtern,
                        InitContext = initDecl.initializer(),
                    };
                }
            }
        }
    }
}
