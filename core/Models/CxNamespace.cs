using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Models
{
    public class CxNamespace
    {
        public string Namespace { get; set; } = string.Empty;
        public List<CxNamespace> NestedNamespaces { get; init; } = new List<CxNamespace>();
        public List<CxGlobalVariable> GlobalVariables { get; init; } = new List<CxGlobalVariable>();
        public List<CxFunction> Functions { get; init; } = new List<CxFunction>();
        public List<CxClass> Classes { get; init; } = new List<CxClass>();
    }
}
