using System;
using System.Collections.Generic;
using System.Text;

namespace cppcx.Core.Models
{
    public class CxClass
    {
        public string Name { get; set; }
        public List<CxMemberFunction> Functions { get; init; } = new List<CxMemberFunction>();
        public List<CxField> Fields { get; init; } = new List<CxField>();
        public List<CxClass> NestedClasses { get; init; } = new List<CxClass>();
        public bool IsSealed { get; set; } = false;
        public bool IsFinal { get; set; } = false;
    }
}
